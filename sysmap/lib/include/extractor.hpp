#ifndef __ADAFS_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_HPP__

#include <string>
#include <unordered_map>
#include <memory>

#include "utils.hpp"

namespace adafs {

struct Extractor_Set;

struct Extractor {

    using extractor_ptr = std::unique_ptr<Extractor>();

    Extractor(const std::string& name) : m_name{name} {}

    virtual ~Extractor() {}

    virtual void load(Extractor_Set& findings) = 0;
    virtual void store(Extractor_Set& findings, const std::string& dbname) = 0;

    std::string name() const noexcept { return m_name; }

    static void registrate(const std::string& name, extractor_ptr* extr)
    {
        registry()[name] = extr;
        for (const auto& kvp : registry()) {
            utils::log::logging::debug() << kvp.first << "\n";
        }
    }

    static std::unique_ptr<Extractor> instantiate(const std::string& name)
    {
        if (registry().empty()) {
            utils::log::logging::debug() << "[EXTRACTOR] registry empty!!!\n";
        }
        auto it = registry().find(name);
        return it == registry().end() ? nullptr : (it->second)();
    }

    template<typename Extr>
    struct Registrar
    {
        explicit Registrar(const std::string& name)
        {
            utils::log::logging::debug() << "CALLING REGISTRATE: " << name << "\n";
            Extractor::registrate(name, &Extr::create);
        }

        // forbid copy construction
        Registrar(const Registrar&) = delete;
        // forbid copy assignment
        Registrar& operator=(const Registrar&) = delete;
    };

    private:
    const std::string m_name;
    static std::unordered_map<std::string, extractor_ptr*> & registry();

};

} /* closing namespace adafs */

#endif /* __ADAFS_EXTRACTOR_HPP__ */
