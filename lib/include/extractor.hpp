#ifndef __ADAFS_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_HPP__

/***
 * @file extractor.hpp
 * Abstract base class defining the extractor interface.
 *
 * The main purpose of the interface is to provide load() and store() methods.
 * Arbritarilly the name of the extractor can be asked.
 */

#include "utils.hpp"

#include <string>
#include <unordered_map>
#include <memory>

namespace adafs
{

/// forward declaration @see Extractor_Set
struct Extractor_Set;

/**
 * @class Extractor
 *  Abstract extractor base class.
 */
struct Extractor
{
    using extractor_ptr = std::unique_ptr<Extractor>(*)();
    using registry_map = std::unordered_map<std::string, extractor_ptr>;

    /**
     * Instantiate a registered Extractor.
     *
     * @param name The name of the extractor that should instantiated.
     * @return Returns a unique pointer to the Extractor.
     */
    static std::unique_ptr<Extractor> instantiate(const std::string& name);

    /**
     * Static registry.
     *
     * These method returns the registry map, where each entry corresponse to one registered extractor.
     * @return Returns registry map.
     */
    static registry_map & registry();

    /**
     * Constructor creates a new extrator.
     * @param name The name of the extractor.
     */
    Extractor(const std::string& name) : m_name{name} {}

    /**
     * Destructor
     */
    virtual ~Extractor() = default;

    /**
     * Virtual load methods.
     *
     * Each concrete Extractor implements this method to load the extracted data in the Extractor_Set.
     * @param findings The Extractor_Set where to store the extracted data.
     */
    virtual void load(Extractor_Set& findings) = 0;

    /**
     * Virtual store method.
     * 
     * Each concrete Extractor implements this method to define how to store the extracted data.
     * @param findings Extractor_Set reference where the extracted data is stored.
     * @param dbname The database name where to store the information.
     */
    virtual void store(Extractor_Set& findings, const std::string& dbname) = 0;

    /**
     * Get the name of the extractor.
     * @return Returns the name of the extractor.
     */
    std::string name() const noexcept { return m_name; }

    private:
    const std::string m_name;
};


/**
 * @class Registrar
 * Helper class does registration of extractors during construction time.
 */
struct Registrar
{
    /**
     * Registrar Constructor
     * @param name The name of the Extractor that should be registrated.
     * @param extr A function pointer to a static factory function for that extractor.
     */
    Registrar(const std::string& name, Extractor::extractor_ptr extr);
};

} /* closing namespace adafs */

#endif /* __ADAFS_EXTRACTOR_HPP__ */
