#ifndef __ADAFS_ARRAY_VALUE_HPP__
#define __ADAFS_ARRAY_VALUE_HPP__

#include "value.hpp"
#include "output.hpp"

#include <vector>
#include <memory>

namespace adafs {

struct Array_value : Value {

    Array_value() {}

    ~Array_value() {}

    Array_value(const Array_value&) = delete;

    Array_value operator=(const Array_value&) = delete;

    void add(std::unique_ptr<Value> value);

    virtual std::ostream& write(std::ostream& os, const Output_format format, bool quouted) const override;

    private:
    std::vector<std::unique_ptr<Value>> m_elements;
};

} /* closing namespace adafs */

#endif /* __ADAFS_ARRAY_VALUE_HPP__ */
