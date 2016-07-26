#ifndef __ADAFS_MAP_VALUE_HPP__
#define __ADAFS_MAP_VALUE_HPP__

#include "value.hpp"

#include <map>
#include <memory>

namespace adafs {

    struct Map_value : value {

        Map_value() {}

        ~Map_Value() {}

        Map_value(const Map_value&) = delete;

        Map_value operator=(const Map_value&) = delete;

        void add(const std::string& name, const std::unique_ptr<Value>& value);

        private:
        std::map<std::string, std::unique_ptr<Value>> m_elements;

    };
}

#endif /* __ADAFS_MAP_VALUE_HPP__ */
