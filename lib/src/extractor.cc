#include "extractor.hpp"
#include "utils.hpp"

namespace sysmap
{
    Extractor::registry_map & Extractor::registry()
    {
        static registry_map impl;
        return impl;
    }

    std::unique_ptr<Extractor> Extractor::instantiate(const std::string& name)
    {
        if (registry().empty()) {
            utils::log::logging::error() << "[Extractor] registry is empty!\n";
        }
        auto it = Extractor::registry().find(name);
        return it == Extractor::registry().end() ? nullptr : (it->second)();
    }

    Registrar::Registrar(const std::string& name, Extractor::extractor_ptr extr)
    {
        Extractor::registry()[name] = extr;
        utils::log::logging::debug() << "[Registrar::Registrar] Register extractor: " << name << "\n";
    }

} /* closing namespace sysmap */
