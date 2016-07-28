#ifndef __ADAFS_VALUE_HPP__
#define __ADAFS_VALUE_HPP__

#include <memory>

namespace adafs {

struct Value {

    Value() {}

    virtual ~Value() = default;

};

template<typename T, typename ...Args>
std::unique_ptr<T> make_value(Args&& ...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

} /* closing namespace adafs */

#endif /* __ADAFS_VALUE_HPP__ */
