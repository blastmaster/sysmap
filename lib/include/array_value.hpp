#ifndef __ADAFS_ARRAY_VALUE_HPP__
#define __ADAFS_ARRAY_VALUE_HPP__

#include "value.hpp"
#include "output.hpp"

#include <vector>
#include <memory>

namespace adafs {

struct Array_value : Value {

    using arrayvalue_t = std::vector<std::unique_ptr<Value>>;
    using iterator = arrayvalue_t::iterator;
    using const_iterator = arrayvalue_t::const_iterator;

    Array_value() = default;

    ~Array_value() = default;

    Array_value(const Array_value&) = delete;

    Array_value operator=(const Array_value&) = delete;

    void add(std::unique_ptr<Value> value);

    bool empty() const;

    size_t size() const;

    const Value* operator[](size_t idx) const;

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

    template<typename T>
    const T* get(size_t idx) const
    {
        return dynamic_cast<const T*>(m_elements.at(idx).get());
    }

    virtual void to_json(Writer<OStreamWrapper>& writer) const override;

    virtual std::ostream& write(std::ostream& os, const Output_format format, bool quoted) const override;

    private:
    std::vector<std::unique_ptr<Value>> m_elements;
};

} /* closing namespace adafs */

#endif /* __ADAFS_ARRAY_VALUE_HPP__ */
