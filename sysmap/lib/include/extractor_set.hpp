#ifndef __ADAFS_EXTRACTOR_SET_HPP__
#define __ADAFS_EXTRACTOR_SET_HPP__

#include "extractor.hpp"
#include "value.hpp"

#include <memory>
#include <map>

namespace adafs {

struct Extractor_Set {

    Extractor_Set() {}

    ~Extractor_Set() {}

    // forbid copy construction
    Extractor_Set(const Extractor_Set&) = delete;
    // forbid copy assignment
    Extractor_Set& operator=(const Extractor_Set&) = delete;

    void add_info(const std::string& name, std::unique_ptr<Value> value);

    void add_extractor(const std::string& name, std::shared_ptr<Extractor> extractor);

    private:
        // map of extractors to use name -> extractor
        std::map<std::string, std::shared_ptr<Extractor>> m_extractormap;
        // map of informations gathered from extractors name -> value
        std::map<std::string, std::unique_ptr<Value>> m_infomap;

};


} /* closing namespace adafs */

#endif /* __ADAFS__EXTRACTOR_SET_HPP__ */
