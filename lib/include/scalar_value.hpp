#ifndef __ADAFS_SCALAR_VALUE_HPP__
#define __ADAFS_SCALAR_VALUE_HPP__

#include "value.hpp"
#include "output.hpp"

#include <iostream>

namespace adafs {

    /**
     * @class Scalar_value
     * Represents a simple scalar value type.
     * @tparam T underlying scalar valur type.
     */
    template<typename T>
    struct Scalar_value : Value
    {

        using value_type = T;

        /**
         * Constructor creates a Scalar_value object of the given value.
         * @param Value of type value_type. The scalar value.
         */
        Scalar_value(value_type value) : m_value(std::move(value)) {}

        /**
         * Destructor default.
         */
        ~Scalar_value() = default;

        /**
         * Forbid copy-construction.
         */
        Scalar_value(const Scalar_value&) = delete;

        /**
         * Forbid copy-assignment.
         */
        Scalar_value operator=(const Scalar_value&) = delete;

        /**
         * Get a const reference to the value of the Scalar_value.
         * @returns The value.
         */
        const value_type& value() const { return m_value; }

        /**
         * Convert the Scalar value to json.
         * @param Writer object, Defines how to write to json.
         */
        virtual void to_json(Writer<OStreamWrapper>& writer) const override;

        /**
        * Writes the Scalar_value to the given stream.
        * @param ostream& defines where to write.
        * @param Output_fromat in which format should the output appear.
        * @param Boolean quoted should the output be quoted or not.
        * @return Returns the stream being written to.
        * @see Output_format
        */
        virtual std::ostream& write(std::ostream& os, const Output_format format, bool quoted) const override
        {
            if (quoted)
                return os << '"' << m_value << '"';
            return os << m_value;
        }

        private:
        value_type m_value;
    };

    // Define type aliases for most common scalar value types.
    using String_value = Scalar_value<std::string>;
    using Uint_value = Scalar_value<uint64_t>;
    using Int_value = Scalar_value<int>;
    using Double_value = Scalar_value<double>;

    // Declare specializations for json
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
