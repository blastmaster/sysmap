#ifndef __ADAFS_MAP_VALUE_HPP__
#define __ADAFS_MAP_VALUE_HPP__

#include "value.hpp"
#include "output.hpp"

#include <map>
#include <memory>

namespace adafs {

    struct Map_value : Value {

        using mapvalue_t = std::map<std::string, std::unique_ptr<Value>>;
        using iterator = mapvalue_t::iterator;
        using const_iterator = mapvalue_t::const_iterator;

        Map_value() = default;

        ~Map_value() = default;

        Map_value(const Map_value&) = delete;

        Map_value operator=(const Map_value&) = delete;

        void add(const std::string& name, std::unique_ptr<Value> value);

        bool empty() const;

        size_t size() const;

        const Value* operator[](const std::string& name) const;

        template<typename T>
        const T* get(const std::string& name) const
        {
            return dynamic_cast<const T*>(this->operator[](name));
        }

        virtual void to_json(Writer<OStreamWrapper>& writer) const override;

        virtual std::ostream& write(std::ostream& os, const Output_format format, bool quouted) const override;


        iterator begin();

        iterator end();

        const_iterator begin() const;
        const_iterator end() const;

        private:
        mapvalue_t m_elements;

    };

} /* closing namespace adafs */

#endif /* __ADAFS_MAP_VALUE_HPP__ */
