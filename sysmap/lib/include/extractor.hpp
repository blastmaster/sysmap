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
 *
 *  More detailed documentation.
 */
struct Extractor
{
    using extractor_ptr = std::unique_ptr<Extractor>(*)();
    using registry_map = std::unordered_map<std::string, extractor_ptr>;

    static std::unique_ptr<Extractor> instantiate(const std::string& name);
    static registry_map & registry();

    /**
     * Constructor creates a new extrator.
     * @param naem of the extractor
     */
    Extractor(const std::string& name) : m_name{name} {}

    /**
     * Destructor
     */
    virtual ~Extractor() = default;

    /**
     * virtual load methods.
     * @param Extractor_Set& the extractor set where to store the extracted data.
     */
    virtual void load(Extractor_Set& findings) = 0;
    virtual void store(Extractor_Set& findings, const std::string& dbname) = 0;

    std::string name() const noexcept { return m_name; }

    private:
    const std::string m_name;
};


/**
 * @class Registrar
 *
 * Helper class does registration of extractors during construction time.
 */
struct Registrar
{
    Registrar(const std::string& name, Extractor::extractor_ptr extr);
};

} /* closing namespace adafs */

#endif /* __ADAFS_EXTRACTOR_HPP__ */
