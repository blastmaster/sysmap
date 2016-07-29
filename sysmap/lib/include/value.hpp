#ifndef __ADAFS_VALUE_HPP__
#define __ADAFS_VALUE_HPP__

#include "output.hpp"

#include <memory>

namespace adafs {

struct Value {

    Value() {}

    virtual ~Value() = default;

    virtual std::ostream& write(std::ostream& os, const Output_format format, bool quouted) const = 0;

};

template<typename T, typename ...Args>
std::unique_ptr<T> make_value(Args&& ...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

} /* closing namespace adafs */

#endif /* __ADAFS_VALUE_HPP__ */
