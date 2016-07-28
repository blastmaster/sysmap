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

} /* closing namespace adafs */
