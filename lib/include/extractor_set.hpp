#ifndef __SYSMAP_EXTRACTOR_SET_HPP__
#define __SYSMAP_EXTRACTOR_SET_HPP__

/**
 * @file extractor_set.hpp
 * Manages selected extractors and holds extracted information.
 *
 * This class is the interface with that an application works with the extractor
 * classes and the information extracted by them.
 */

#include "extractor.hpp"
#include "value.hpp"
#include "output.hpp"

#include <memory>
#include <map>

namespace sysmap {

/**
 * @class Extractor_Set
 * Holds an extractor-map and an infomap containing the extracted information.
 */
struct Extractor_Set
{
    using infomap_t = std::map<std::string, std::unique_ptr<Value>>;
    using extractormap_t = std::map<std::string, std::shared_ptr<Extractor>>;

    /**
     * Constructor
     */
    Extractor_Set() = default;

    /**
     * Destructor
     */
    ~Extractor_Set() = default;

    /**
     * Forbid copy-construction.
     */
    Extractor_Set(const Extractor_Set&) = delete;

    /**
     * Forbid copy-assignment.
     */
    Extractor_Set& operator=(const Extractor_Set&) = delete;

    /**
     * Add an extractor to the extractormap by the given name.
     * @param name The name of the extractor that should added.
     */
    void add_by_tag(const std::string& name);

    /**
     * Add an extracted information value to the infomap.
     * @param name The key entry in the infomap for that info.
     * @param value The actual value that should be added.
     * @see Value
     */
    void add_info(const std::string& name, std::unique_ptr<Value> value);

    /**
     * Add an extractor to the extractormap.
     * @param extractor The extractor that should be added.
     * @see Extractor
     */
    void add(const std::shared_ptr<Extractor>& extractor);

    /**
     * Add an extractor to the extractormap with the given name.
     * @param name The name under which the extractor should stored in the extractormap.
     * @param extractor The extractor that should be stored.
     * @see Extractor
     */
    void add_extractor(const std::string& name, const std::shared_ptr<Extractor>& extractor);

    /**
     * Is the extractormap empty.
     * @return Returns true if the extractormap is empty, false otherwise.
     */
    bool empty() const noexcept { return m_extractormap.empty(); }

    /**
     * Do the extraction, all added extractors in the extractormap will be
     * called to do there extraction.
     */
    void extract();

    /**
     * Save extracted infomap in given database.
     * @param dbname The name of the database.
     */
    void save(const std::string& dbname);

    /**
     * Write extracted infomap.
     * @param os The out-stream where to write to.
     * @param format The format in which should be written.
     * @see Ouptut_format
     */
    void write(std::ostream& os, const Output_format format);

    /**
     * Get a value in the infomap of expected type.
     * @tparam T The expected type of the value.
     * @param name The name of the value in the infomap.
     * @return Returns a pointer to the value of the expected type.
     */
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


} /* closing namespace sysmap */

#endif /* __SYSMAP__EXTRACTOR_SET_HPP__ */
