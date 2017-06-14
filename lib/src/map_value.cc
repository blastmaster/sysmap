#include "array_value.hpp"
#include "map_value.hpp"
#include "output.hpp"
#include "utils.hpp"
#include "pugixml.hpp"

#include <iostream>
#include <sstream>
#include <typeinfo>


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

    bool Map_value::empty() const
    {
        return m_elements.empty();
    }

    size_t Map_value::size() const
    {
        return m_elements.size();
    }

    const Value* Map_value::operator[](const std::string& name) const
    {
        auto it = m_elements.find(name);
        if (it == m_elements.end()) {
            return nullptr;
        }
        return it->second.get();
    }

    void Map_value::to_json(Writer<OStreamWrapper>& writer) const
    {
        writer.StartObject();
        for (const auto& kvp : m_elements) {
            writer.Key(kvp.first.c_str());
            kvp.second->to_json(writer);
        }
        writer.EndObject();
    }

    void Map_value::to_xml(pugi::xml_node& node) const
    {
        for (const auto& kvp : m_elements) {
            const char *c;
            c = kvp.first.c_str();
            pugi::xml_node child_node = node.append_child(c);
            kvp.second->to_xml(child_node);
        }
    }

    std::ostream& Map_value::write(std::ostream& os, const Output_format format, bool quoted) const
    {
        //static bool first = true;
        return os;
    }


    Map_value::iterator Map_value::begin()
    {
        return m_elements.begin();
    }

    Map_value::iterator Map_value::end()
    {
        return m_elements.end();
    }

    Map_value::const_iterator Map_value::begin() const
    {
        return m_elements.begin();
    }

    Map_value::const_iterator Map_value::end() const
    {
        return m_elements.end();
    }

} /* closing namespace adafs */
