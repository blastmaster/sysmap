#include <iostream>
#include "extractor_set.hpp"

namespace adafs {

    void Extractor_Set::add_info(const std::string& name, std::unique_ptr<Value> value)
    {
        if (!value) {
            std::cerr << "[Extractor_Set::add_info] Error, no value!\n";
            return;
        }

        m_infomap.emplace(std::move(name), std::move(value));
    }


    void Extractor_Set::add_extractor(const std::string& name, std::shared_ptr<Extractor> extractor)
    {
        if (!extractor) {
            std::cerr << "[Extractor_Set::add_extractor] Error, no extractor!\n";
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
            std::cerr << "Infomap is empty, extract data before trying to write!\n";
            return;
        }

        switch (format) {

            case Output_format::XML:
                os << XMLOutput::xml_preamble << "\n"
                    << XMLOutput::xml_root_element_open << "\n";
                for (const auto& kv_info : m_infomap) {
                    os << "<" << kv_info.first << ">\n";
                    kv_info.second->write(os, format, true);
                    os << "</" << kv_info.first << ">\n";
                }
                os << XMLOutput::xml_root_element_close << "\n";
                break;
        }

    }


} /* closing namespace adafs */
