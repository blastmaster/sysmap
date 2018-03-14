#ifndef __SYSMAP_MAP_VALUE_HPP__
#define __SYSMAP_MAP_VALUE_HPP__

#include "value.hpp"
#include "output.hpp"
#include "pugixml.hpp"
#include "yaml-cpp/yaml.h"

#include <map>
#include <memory>

namespace sysmap {
    /**
     * @class Map_value
     */
    struct Map_value : Value {

        using mapvalue_t = std::map<std::string, std::unique_ptr<Value>>;
        using iterator = mapvalue_t::iterator;
        using const_iterator = mapvalue_t::const_iterator;

        /**
         * Default constructor.
         */
        Map_value() = default;

        /**
         * Default destructor.
         */
        ~Map_value() = default;

        /**
         * Forbid copy-construction.
         */
        Map_value(const Map_value&) = delete;

        /**
         * Forbid copy-assignement
         */
        Map_value operator=(const Map_value&) = delete;

        /**
         * Add a value to the map.
         * @param name The name of the value.
         * @param value The value of the element.
         */
        void add(const std::string& name, std::unique_ptr<Value> value);

        /**
         * Check if the map is empty.
         * @return Boolean, true if empty, false otherwise.
         */
        bool empty() const;

        /**
         * Get the size of the map.
         * @return Returns the number of elements in the map.
         */
        size_t size() const;

        /**
         * Get the value in the map of the given name.
         * @param name The name of the element in the map to get.
         * @return Returns a pointer to the value in the map. Or nullptr.
         */
        const Value* operator[](const std::string& name) const;

        /**
         * Get the value in the map of the given name.
         * @tparam T The expected type of the value.
         * @param name The name of the element in the map to get.
         * @return Returns a pointer to the value of the map of type T or a nullptr.
         */
        template<typename T>
        const T* get(const std::string& name) const
        {
            return dynamic_cast<const T*>(this->operator[](name));
        }

       /**
        * Converts Map_value to a json object.
        * @param Writer object. Defines how to write to json.
        */
        virtual void to_json(Writer<OStreamWrapper>& writer) const override;

       /**
        * Appends Map_value to a xml object.
        */
        virtual void to_xml(pugi::xml_node& node) const;

       /**
        * Appends Map_value to a yaml object.
        */
        virtual void to_yaml(YAML::Emitter& node) const;

       /**
        * Writes the Map_value to the given stream.
        * @param ostream& defines where to write.
        * @param Output_fromat in which format should the output appear.
        * @param Boolean quoted should the output be quoted or not.
        * @return Returns the stream being written to.
        * @see Output_format
        */
        virtual std::ostream& write(std::ostream& os, const Output_format format, bool quouted) const override;


       /**
        * Get an iterator to the first element of the map.
        * @return iterator to first element.
        */
        iterator begin();

       /**
        * Get an iterator the one element behind the last element of the map.
        * @return iterator one past the last element.
        */
        iterator end();

       /**
        * Get a constant iterator to the first element of the map.
        * @return const iterator to first element.
        */
        const_iterator begin() const;

       /**
        * Get a constant iterator one element behind the last element of the map.
        * @return const iterator one past the last element.
        */
        const_iterator end() const;

        private:
        mapvalue_t m_elements;

    };

} /* closing namespace sysmap */

#endif /* __SYSMAP_MAP_VALUE_HPP__ */
