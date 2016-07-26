#ifndef __ADAFS_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_HPP__

#include "extractor_set.hpp"

namespace adafs {

struct Extractor {

    Extractor(const std::string& name) : m_name{name} {}

    virtual ~Extractor() {}

    virtual void load(Extractor_Set& findings) = 0;

    std::string name() const { return m_name; }

    private:
    std::string m_name;

};


} /* closing namespace adafs */

#endif /* __ADAFS_EXTRACTOR_HPP__ */
