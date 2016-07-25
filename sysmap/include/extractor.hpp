#ifndef __ADAFS_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_HPP__

namespace adafs {

struct Extractor {

    Extractor(const std::string& name) : m_name{name} {}

    virtual ~Extractor() {}

    virtual void load() = 0;
    virtual void store() = 0;

    std::string name() const { return m_name; }

    private:
    std::string m_name;

};


} /* closing namespace adafs */

#endif /* __ADAFS_EXTRACTOR_HPP__ */
