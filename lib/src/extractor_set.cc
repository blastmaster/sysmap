#include "extractor_set.hpp"
#include "utils.hpp"
#include "pugixml.hpp"

namespace adafs {

    void Extractor_Set::add_info(const std::string& name, std::unique_ptr<Value> value)
    {
        if (!value) {
            adafs::utils::log::logging::error() << "[Extractor_Set::add_info] Error, no value!\n";
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
            adafs::utils::log::logging::error() << "[Extractor_Set::add] Error, no extractor\n";
            return;
        }

        m_extractormap.emplace(extractor->name(), extractor);
    }


    void Extractor_Set::add_extractor(const std::string& name,
                                      const std::shared_ptr<Extractor>& extractor)
    {
        if (!extractor) {
            adafs::utils::log::logging::error() << "[Extractor_Set::add_extractor] Error, no extractor!\n";
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


    void Extractor_Set::save(const std::string& dbname)
    {
        for (const auto& kv_extr : m_extractormap) {
            auto extractor = kv_extr.second.get();
            extractor->store(*this, dbname);
        }
    }


    void Extractor_Set::write(std::ostream& os, const Output_format format)
    {
        if (m_infomap.empty()) {
            adafs::utils::log::logging::error() << "[Extractor_Set::write] Infomap is empty, extract data before trying to write!\n";
            return;
        }

        switch (format)
        {
            case Output_format::XML:

                pugi::xml_document doc;
                for (const auto& kv_info : m_infomap) {
                    pugi::xml_node node = doc.append_child(kv_info.first); //TODO: Nodename must change for every kv_info
                    kv_info.second->to_xml(node);
//                    XML_Writer::make_tag(doc, kv_info.first, kv_info.second.get());
                }
                doc.save(os);
                break;

            case Output_format::JSON:

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
    }


    const Value* Extractor_Set::get_value(const std::string& name)
   {
        auto it = m_infomap.find(name);
        if (it == m_infomap.end()) {
            return nullptr;
        }
        return it->second.get();
    }

} /* closing namespace adafs */
