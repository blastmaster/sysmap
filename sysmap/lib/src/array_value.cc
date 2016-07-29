#include <iostream>
#include "array_value.hpp"

namespace adafs {

    void Array_value::add(std::unique_ptr<Value> value)
    {
        if (!value) {
            std::cerr << "[Array_value::add] Error, no value!\n";
            return;
        }

        m_elements.emplace_back(std::move(value));
    }

    std::ostream& Array_value::write(std::ostream& os, const Output_format format, bool quoted) const
    {
        switch (format) {
            case Output_format::XML :
                if (quoted) {
                    os << '"';
                }
                for (const auto& v : m_elements) {
                    if (quoted)
                        v->write(os, format, false);
                    else
                        v->write(os, format, quoted);
                }
                if (quoted) {
                    os << '"';
                }
        }

        return os;
    }

} /* closing namespace adafs */
