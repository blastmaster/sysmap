#include <iostream>
#include "map_value.hpp"

namespace adafs {

    void Map_value::add(const std::string& name, std::unique_ptr<Value> value)
    {
        if (!value) {
            std::cerr << "[Map_value::add] Error, no value!\n";
            return;
        }

        m_elements.emplace(std::move(name), std::move(value));
    }

    std::ostream& Map_value::write(std::ostream& os, const Output_format format, bool quoted) const
    {
        static bool first = true;
        switch (format) {
            case Output_format::XML :
                for (const auto& kvp : m_elements) {
                    if (first) {
                        os << "<object ";
                        first = false;
                        kvp.second->write(os, format, quoted);
                        os << "/>\n";
                    }
                    else {
                        os << " " << kvp.first << "=";
                        kvp.second->write(os, format, quoted);
                    }
                }
                first = true;
        }
        return os;
    }

} /* closing namespace adafs */
