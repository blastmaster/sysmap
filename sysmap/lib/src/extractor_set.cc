#include "extractor_set.hpp"
#include "utils.hpp"

namespace adafs {

    void Extractor_Set::add_info(const std::string& name, std::unique_ptr<Value> value)
    {
        if (!value) {
            adafs::utils::log::logging::error() << "[Extractor_Set::add_info] Error, no value!\n";
            return;
        }

        m_infomap.emplace(std::move(name), std::move(value));
    }


    void Extractor_Set::add_extractor(const std::string& name, std::shared_ptr<Extractor> extractor)
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
            auto extractor = kv_extr.second.get();
            extractor->load(*this);
        }
    }

    void Extractor_Set::write(std::ostream& os, const Output_format format)
    {
        if (m_infomap.empty()) {
            adafs::utils::log::logging::error() << "[Extractor_Set::write] Infomap is empty, extract data before trying to write!\n";
            return;
        }

        switch (format) {

            case Output_format::XML:
                os << XML_Writer::xml_preamble << "\n"
                    << XML_Writer::xml_root_element_open << "\n";
                for (const auto& kv_info : m_infomap) {
                    XML_Writer::make_tag(os, kv_info.first, kv_info.second.get());
                }
                os << XML_Writer::xml_root_element_close << "\n";
                break;
        }

    }


} /* closing namespace adafs */
