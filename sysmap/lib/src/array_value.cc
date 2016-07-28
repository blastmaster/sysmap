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

} /* closing namespace adafs */
