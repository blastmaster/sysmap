#ifndef __ADAFS_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_HPP__

#include <string>
#include <unordered_map>
#include <memory>

#include "utils.hpp"

namespace adafs
{

struct Extractor_Set;

struct Extractor
{
    using extractor_ptr = std::unique_ptr<Extractor>(*)();
    using registry_map = std::unordered_map<std::string, extractor_ptr>;

    static std::unique_ptr<Extractor> instantiate(const std::string& name);
    static registry_map & registry();

    Extractor(const std::string& name) : m_name{name} {}

    virtual ~Extractor() = default;
    virtual void load(Extractor_Set& findings) = 0;
    virtual void store(Extractor_Set& findings, const std::string& dbname) = 0;

    std::string name() const noexcept { return m_name; }

    private:
    const std::string m_name;
};


struct Registrar
{
    Registrar(const std::string& name, Extractor::extractor_ptr extr);
};

} /* closing namespace adafs */

#endif /* __ADAFS_EXTRACTOR_HPP__ */
