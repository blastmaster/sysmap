#ifndef __ADAFS_SCALAR_VALUE_HPP__
#define __ADAFS_SCALAR_VALUE_HPP__

#include "value.hpp"

namespace adafs {

    template<typename T>
    struct Scalar_value : value {

        using value_type = typename T;

        Scalar_value() {}

        ~Scalar_value() {}

        Scalar_value(const Scalar_value&) = delete;

        Scalar_value operator=(const Scalar_value&) = delete;

        const value_type& value() const { return m_value; }

        private:
        value_type m_value;

    };

    /**
     * Define type aliases for most common scalar value types.
     */
    using String_value = Scalar_value<std::string>;
    using Uint_value = Scalar_value<uint64_t>;
    using Int_value = Scalar_value<int>;
    using Double_value = Scalar_value<double>;

} /* closing namespace adafs */

#endif /* __ADAFS_SCALAR_VALUE_HPP__ */
