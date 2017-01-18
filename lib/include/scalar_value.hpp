#ifndef __ADAFS_SCALAR_VALUE_HPP__
#define __ADAFS_SCALAR_VALUE_HPP__

#include "value.hpp"
#include "output.hpp"

#include <iostream>

namespace adafs {

    template<typename T>
    struct Scalar_value : Value
    {

        using value_type = T;

        Scalar_value(value_type value) : m_value(std::move(value)) {}

        ~Scalar_value() = default;

        Scalar_value(const Scalar_value&) = delete;

        Scalar_value operator=(const Scalar_value&) = delete;

        const value_type& value() const { return m_value; }

        virtual void to_json(Writer<OStreamWrapper>& writer) const override;

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

    /**
     * Declare specializations for json
     */
    template<>
    void String_value::to_json(Writer<OStreamWrapper>&) const;
    template<>
    void Uint_value::to_json(Writer<OStreamWrapper>&) const;
    template<>
    void Int_value::to_json(Writer<OStreamWrapper>&) const;
    template<>
    void Double_value::to_json(Writer<OStreamWrapper>&) const;

} /* closing namespace adafs */

#endif /* __ADAFS_SCALAR_VALUE_HPP__ */
