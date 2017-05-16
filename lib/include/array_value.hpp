#ifndef __ADAFS_ARRAY_VALUE_HPP__
#define __ADAFS_ARRAY_VALUE_HPP__

#include "value.hpp"
#include "output.hpp"
#include "pugixml.hpp"

#include <vector>
#include <memory>

namespace adafs {

/**
 * @class Array_value
 * List-like container class for extracted data.
 */
struct Array_value : Value {

    using arrayvalue_t = std::vector<std::unique_ptr<Value>>;
    using iterator = arrayvalue_t::iterator;
    using const_iterator = arrayvalue_t::const_iterator;

    /**
     * Default constructor.
     */
    Array_value() = default;

    /**
     * Default destructor.
     */
    ~Array_value() = default;

    /**
     * Forbid copy-construction.
     */
    Array_value(const Array_value&) = delete;

    /**
     * Forbid copy-assignement.
     */
    Array_value operator=(const Array_value&) = delete;

    /**
     * Add a value to the array.
     * @param a Value unique_ptr
     * @see Value
     */
    void add(std::unique_ptr<Value> value);

    /**
     * Check if this array is empty.
     * @return Boolean, true if empty, false otherwise.
     */
    bool empty() const;

    /**
     * Get the size of the array, where the size is the number of elements.
     * @return size_t size-value
     */
    size_t size() const;

    /**
     * Subscript-Operator to enable indexing on Array_value objects.
     * @param size_t idx, index to the element in the Array_value.
     * @return Value pointer
     */
    const Value* operator[](size_t idx) const;

    /**
     * Get an iterator to the first element of the array.
     * @return iterator to first element.
     */
    iterator begin();

    /**
     * Get an iterator the one element behind the last element of the array.
     * @return iterator one past the last element.
     */
    iterator end();

    /**
     * Get a constant iterator to the first element of the array.
     * @return const iterator to first element.
     */
    const_iterator begin() const;

    /**
     * Get a constant iterator one element behind the last element of the array.
     * @return const iterator one past the last element.
     */
    const_iterator end() const;

    /**
     * Get an element of a specific type at index idx.
     * @tparam Type of the element addressed by the index. Must be a derived type of Value.
     * @param size_t idx, index to the element in the Array_value.
     * @return Pointer to element of type T.
     * @see Value
     */
    template<typename T>
    const T* get(size_t idx) const
    {
        return dynamic_cast<const T*>(m_elements.at(idx).get());
    }

    /**
     * Converts Array_value to a json array.
     * @param Writer object. Defines how to write to json.
     */
    virtual void to_json(Writer<OStreamWrapper>& writer) const override;

    virtual void to_xml(pugi::xml_node& node) const;
    /**
     * Writes the Array_value to the given stream.
     * @param ostream& defines where to write.
     * @param Output_fromat in which format should the output appear.
     * @param Boolean quoted should the output be quoted or not.
     * @return Returns the stream being written to.
     * @see Output_format
     */
    virtual std::ostream& write(std::ostream& os, const Output_format format, bool quoted) const override;

    private:
    std::vector<std::unique_ptr<Value>> m_elements;
};

} /* closing namespace adafs */

#endif /* __ADAFS_ARRAY_VALUE_HPP__ */
