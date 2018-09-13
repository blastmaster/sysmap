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
        const auto hostname = get_hostname();

        //creates a select statement which returns column_return in a given table
        //where column_name equals the given value
        auto getID = [] (std::string table,
                         std::string column_return,
                         std::string column_name,
                         std::string value) {
            return "select " + column_return + " from " + table
                 + " where " + column_name + " = '" + value + "'";
        };

        //insert into Hosttable, then get HostID from database
        os << "insert or ignore into Hosttable (Hostname) values ('" << hostname << "');\n";
        auto HostID = getID("Hosttable", "HostID", "Hostname", hostname);

        for (const auto& kv_extr : m_infomap) {
            //insert into Extractortable, then get EID from database
            os << "insert or ignore into Extractortable (name) values ('"
               << kv_extr.first.c_str() << "');\n";
            std::string eid = "select EID from Extractortable where Name = '" + kv_extr.first + "'";

            //init jsonstring and fill it with data
            std::stringstream jsonstring;
            OStreamWrapper osw(jsonstring);
            Writer<OStreamWrapper> writer(osw);
            kv_extr.second->to_json(writer);

            //insert into Datatable, then get DID from database
            os << "insert or ignore into Datatable (EID, Data) values ((" << eid << "), '"
               << jsonstring.str() << "');\n";
            auto DID = getID("Datatable", "DID", "Data", jsonstring.str());

            //insert into Host2Data
            os << "insert into Host2Data (HostID, DID) select (" << HostID << "), (" << DID
                << ") where not exists (select 1 from Host2Data where HostID = ("
                << HostID << ") AND DID = (" << DID << "));\n";
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

} /* closing namespace sysmap */
