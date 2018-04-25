#include "extractor_set.hpp"
#include "utils.hpp"
#include "pugixml.hpp"
#include "yaml-cpp/yaml.h"
#include <cassert>

namespace sysmap {

    void Extractor_Set::add_info(const std::string& name, std::unique_ptr<Value> value)
    {
        if (!value) {
            sysmap::utils::log::logging::error() << "[Extractor_Set::add_info] Error, no value!\n";
            return;
        }

        m_infomap.emplace(std::move(name), std::move(value));
    }

    void Extractor_Set::add_by_tag(const std::string& name)
    {
        std::shared_ptr<Extractor> extractor = Extractor::instantiate(name);
        if (!extractor) {
            utils::log::logging::error() << "[Extracotr_set::by_tag] Error, no extractor found with tag: [" << name << "]\n";
            return;
        }

        m_extractormap.emplace(std::move(name), std::move(extractor));
    }


    void Extractor_Set::add(const std::shared_ptr<Extractor>& extractor)
    {
        if (!extractor) {
            sysmap::utils::log::logging::error() << "[Extractor_Set::add] Error, no extractor\n";
            return;
        }

        m_extractormap.emplace(extractor->name(), extractor);
    }


    void Extractor_Set::add_extractor(const std::string& name,
                                      const std::shared_ptr<Extractor>& extractor)
    {
        if (!extractor) {
            sysmap::utils::log::logging::error() << "[Extractor_Set::add_extractor] Error, no extractor!\n";
            return;
        }

        m_extractormap.emplace(std::move(name), extractor);
    }


    void Extractor_Set::extract()
    {
        for (const auto& kv_extr : m_extractormap) {
            // why not
            // kv_extr.second->load(*this);
            auto extractor = kv_extr.second.get();
            extractor->load(*this);
        }
    }

    std::string get_hostname()
    {
        std::string host;
        auto prog = utils::exec::which("hostname");
        auto read_host = [&host](const std::string& s) {
                            if (!s.empty()) {
                                host = s;
                                return true;
                            }
                            return false;
                        };
        if (!prog.empty()) {
            auto res = utils::exec::for_each_line(prog, {}, read_host);
            if (res) {
                return host;
            }
        } // if we cannot use the hostname binary try to read Hostname from /proc
        else {
            auto res = utils::file::for_each_line("/proc/sys/kernel/hostname", read_host);
            if (res) {
                return host;
            }
        }

        return host;
    }


    void Extractor_Set::toSQL(std::ostream& os){
        //TODO: instead of path parameter the  --output flag should handle writing to file
        /* assert(&path.back() == std::string("/")); */
        auto hostname = get_hostname();
        /* auto filename = path + hostname + ".sql"; */
        /* std::ofstream file(filename); */

        os << "insert into Hosttable (Hostname) values ('" << hostname << "');\n";

        //returns max of given row at given table, which is the last inserted column
        //TODO: only works on a freshly created db
        auto getID = [] (std::string table, std::string rowName) { 
            return "select " + rowName + " from " + table + " where " + rowName + " = " +
               "(select max(" + rowName + ") from " + table + ")" ;
        };

        auto HostID = getID("Hosttable", "HostID");

        for (const auto& kv_extr : m_infomap) {
            os << "insert into Extractortable (name) values ('" << kv_extr.first.c_str() << "');\n";

            std::stringstream jsonstring;
            OStreamWrapper osw(jsonstring);
            Writer<OStreamWrapper> writer(osw);
            kv_extr.second->to_json(writer);

            std::string eid = "select EID from Extractortable where Name = '" + kv_extr.first + "'";

            os << "insert into Datatable (EID, Data) values ((" << eid << "), '" << jsonstring.str() << "');\n";

            auto DID = getID("Datatable", "DID");

            os << "insert into Host2Data (HostID, DID) values ((" << HostID << "), (" << DID << "));\n";
        }
    }

    void Extractor_Set::save(const std::string& dbname)
    {
        auto db = initDB(dbname);
        auto name = get_hostname();
        auto exists = 0;
        try {
            //kalipso: you should just put a unique constraint on Hostname,
            //then do INSERT OR IGNORE INTO Hosttable (Hostname) VALUES (?)
            db << "begin;";
            db << "select IFNULL((select HostID from Hosttable where Hostname = ?), 0);"
                << name
                >> exists;

            if(!exists){
                db << "insert into Hosttable (Hostname) values (?);"
                    << name;
                exists = db.last_insert_rowid();
            }

            auto HostID = exists;

            for (const auto& kv_extr : m_infomap) {
                db << "select IFNULL((select EID from Extractortable where Name = ? limit 1), 0);"
                    << kv_extr.first.c_str()
                    >> exists;
                if(!exists){
                    db << "insert into Extractortable (name) values (?);"
                        << kv_extr.first.c_str();
                }

                std::stringstream os;
                OStreamWrapper osw(os);
                Writer<OStreamWrapper> writer(osw);
                kv_extr.second->to_json(writer);

                std::string eid;
                db << "select EID from Extractortable where Name = ?;"
                    << kv_extr.first
                    >> eid;

                db << "insert into Datatable (EID, Data) values (?, ?);"
                    << eid
                    << os.str();

                auto DID = db.last_insert_rowid();

                db << "insert into Host2Data (HostID, DID) values (?, ?);"
                    << HostID
                    << DID;

            }
        db << "commit;";
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }


    void Extractor_Set::write(std::ostream& os, const Output_format format)
    {
        if (m_infomap.empty()) {
            sysmap::utils::log::logging::error() << "[Extractor_Set::write] Infomap is empty,"
               << " extract data before trying to write!\n";
            return;
        }

        switch (format)
        {
            case Output_format::XML:
            {
                pugi::xml_document doc;
                for (const auto& kv_info : m_infomap) {
                    const char *c;
                    c = kv_info.first.c_str();
                    pugi::xml_node node = doc.append_child(c);
                    kv_info.second->to_xml(node);
                }
                doc.save(os);
                break;

            }
            case Output_format::JSON:
            {
                OStreamWrapper osw(os);
                Writer<OStreamWrapper> writer(osw);
                writer.StartObject();
                for (const auto& kv_info : m_infomap) {
                    writer.Key(kv_info.first.c_str());
                    kv_info.second->to_json(writer);
                }
                writer.EndObject();
                break;
            }

            case Output_format::YAML:
            {
                YAML::Emitter output;
                output << YAML::BeginMap;
                for (const auto& kv_info : m_infomap) {
                    output << YAML::Key << kv_info.first;
                    output << YAML::Value;
                    kv_info.second->to_yaml(output);
                }
                output << YAML::EndMap;
                os << output.c_str();
                break;
            }

            case Output_format::SQLITE3:
            {
                // /scratch/s8946413/querytest
                auto dbname = "sysmap.db";
                save(dbname);
                break;

            }
            case Output_format::SQL:
            {
                toSQL(os);
            }
        }
    }


    const Value* Extractor_Set::get_value(const std::string& name)
   {
        auto it = m_infomap.find(name);
        if (it == m_infomap.end()) {
            return nullptr;
        }
        return it->second.get();
    }

    sqlite::database Extractor_Set::initDB(const std::string& dbname){
        sqlite::database db(dbname);

      db << "begin;"; // begin a transaction ...
      //Init Hosttable
      db <<
         "create table if not exists Hosttable ("
         "   HostID integer primary key autoincrement not null,"
         "   Hostname text"
         ");";

      //Init Host2Data
      db <<
         "create table if not exists Host2Data ("
         "   HostID integer,"
         "   DID integer"
         ");";

      //Init Datatable
      db <<
         "create table if not exists Datatable ("
         "   DID integer primary key autoincrement not null,"
         "   EID integer,"
         "   Data text"
         ");";

      //Init Extractortable
      db <<
         "create table if not exists Extractortable ("
         "   EID integer primary key autoincrement not null,"
         "   Name text"
         ");";
      db << "commit;"; // begin a transaction ...
      return db;
    }


} /* closing namespace sysmap */
