#ifndef __ADAFS_EXTRACTOR_SET_HPP__
#define __ADAFS_EXTRACTOR_SET_HPP__

#include "extractor.hpp"
#include "value.hpp"
#include "output.hpp"

#include <memory>
#include <map>

namespace adafs {

struct Extractor_Set
{
    using infomap_t = std::map<std::string, std::unique_ptr<Value>>;
    using extractormap_t = std::map<std::string, std::shared_ptr<Extractor>>;

    Extractor_Set() = default;

    ~Extractor_Set() = default;

    // forbid copy construction
    Extractor_Set(const Extractor_Set&) = delete;
    // forbid copy assignment
    Extractor_Set& operator=(const Extractor_Set&) = delete;

    void by_tag(const std::string& name);

    void add_info(const std::string& name, std::unique_ptr<Value> value);

    void add(const std::shared_ptr<Extractor>& extractor);

    void add_extractor(const std::string& name, const std::shared_ptr<Extractor>& extractor);

    bool empty() const noexcept { return m_extractormap.empty(); }

    void extract();

    void save(const std::string& dbname);

    void write(std::ostream& os, const Output_format format);

    template<typename T>
    const T* get(const std::string& name)
    {
        return dynamic_cast<const T*>(get_value(name));
    }

    private:

    const Value* get_value(const std::string& name);

    // map of extractors to use name -> extractor
    extractormap_t m_extractormap;
    // map of informations gathered from extractors name -> value
    infomap_t m_infomap;
};


} /* closing namespace adafs */

#endif /* __ADAFS__EXTRACTOR_SET_HPP__ */
