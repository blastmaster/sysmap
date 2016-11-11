#ifndef __ADAFS_SCALAR_VALUE_HPP__
#define __ADAFS_SCALAR_VALUE_HPP__

#include "value.hpp"
#include "output.hpp"

#include <iostream>

namespace adafs {

    template<typename T>
    struct Scalar_value : Value {

        using value_type = T;

        Scalar_value(value_type value) : m_value(std::move(value)) {}

        ~Scalar_value() {}

        Scalar_value(const Scalar_value&) = delete;

        Scalar_value operator=(const Scalar_value&) = delete;

        const value_type& value() const { return m_value; }

        virtual std::ostream& write(std::ostream& os, const Output_format format, bool quoted) const override
        {
            if (quoted)
                return os << '"' << m_value << '"';
            return os << m_value;
        }

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
    using Ushort_value = Scalar_value<unsigned short>;
    using Uchar_value = Scalar_value<unsigned char>;
    using Float_value = Scalar_value<float>;

} /* closing namespace adafs */

#endif /* __ADAFS_SCALAR_VALUE_HPP__ */
