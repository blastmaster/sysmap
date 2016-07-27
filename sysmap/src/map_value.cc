#include "map_value.hpp"

namespace adafs {

    void Map_value::add(const std::string& name, const std::unique_ptr<Value>& value)
    {
        if (!value) {
            std::cerr << "[Map_value::add] Error, no value!\n";
            return;
        }

        m_elements.emplace(std::move(name), std::move(value));
    }


} /* closing namespace adafs */
