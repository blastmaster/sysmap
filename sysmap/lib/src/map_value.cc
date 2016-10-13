#include <iostream>
#include <sstream>
#include <typeinfo>

#include "array_value.hpp"
#include "map_value.hpp"
#include "output.hpp"
#include "utils/log.hpp"

namespace adafs {

    void Map_value::add(const std::string& name, std::unique_ptr<Value> value)
    {
        if (!value) {
            adafs::utils::log::logging::error() << "[Map_value::add] Error, no value!\n";
            return;
        }

        m_elements.emplace(std::move(name), std::move(value));
    }

    bool is_array_value(const Value* value)
    {
        const auto& ti = typeid(*value);
        return ti == typeid(Array_value);
    }

    std::ostream& Map_value::write(std::ostream& os, const Output_format format, bool quoted) const
    {
        static bool first = true;
        switch (format) {
            case Output_format::XML :
                XML_Writer xml{os};
                std::ostringstream tmp(std::ios_base::ate);
                for (const auto& kvp : m_elements) {
                    const auto v = kvp.second.get();
                    if (!first && is_array_value(v)) {
                        XML_Writer::make_tag(tmp, kvp.first, v);
                        continue;
                    }
                    xml.write_xml_attribute(kvp.first, v);
                }
                first = false;
                if (!tmp.str().empty()) {
                    xml.~XML_Writer();
                    os << tmp.str();
                }
        }

        return os;
    }

} /* closing namespace adafs */
