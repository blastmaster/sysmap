#include "array_value.hpp"
#include "output.hpp"
#include "pugixml.hpp"

#include <iostream>


namespace adafs {

    void Array_value::add(std::unique_ptr<Value> value)
    {
        if (!value) {
            std::cerr << "[Array_value::add] Error, no value!\n";
            return;
        }

        m_elements.emplace_back(std::move(value));
    }

    bool Array_value::empty() const
    {
        return m_elements.empty();
    }

    size_t Array_value::size() const
    {
        return m_elements.size();
    }

    const Value* Array_value::operator[](size_t idx) const
    {
        return m_elements[idx].get();
    }

    void Array_value::to_json(Writer<OStreamWrapper>& writer) const
    {
        writer.StartArray();
        for (const auto& v : m_elements) {
            v->to_json(writer);
        }
        writer.EndArray();
    }

    void Array_value::to_xml(pugi::xml_node& node) const
    {
        for (const auto& v : m_elements) {
            v->to_xml(node);
        }
    }

    std::ostream& Array_value::write(std::ostream& os, const Output_format format, bool quoted) const
    {
        switch (format) {
            case Output_format::XML:
                for (const auto& v : m_elements) {
                    v->write(os, format, quoted);
                }
        }

        return os;
    }

    Array_value::iterator Array_value::begin()
    {
        return m_elements.begin();
    }

    Array_value::iterator Array_value::end()
    {
        return m_elements.end();
    }

    Array_value::const_iterator Array_value::begin() const
    {
        return m_elements.begin();
    }

    Array_value::const_iterator Array_value::end() const
    {
        return m_elements.end();
    }

} /* closing namespace adafs */
