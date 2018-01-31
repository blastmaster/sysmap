#ifndef __ADAFS_VALUE_HPP__
#define __ADAFS_VALUE_HPP__

#include "output.hpp"

#include "yaml-cpp/yaml.h"
#include "rapidjson/writer.h"
#include "rapidjson/ostreamwrapper.h"

#include <memory>

using namespace rapidjson;

namespace adafs {

// Note: function with deduced return type cannot be virtual!

/**
 * @class Value
 * Value type abstract base class.
 *
 *
 */
struct Value {

    /**
     * Default constructor.
     */
    Value() = default;

    /**
     * Virtual default destructor.
     */
    virtual ~Value() = default;

    /**
     * Gets a value of given type T.
     * @tparam T Type of the value expected.
     * @return Returns a pointer of type T to the value.
     */
    template<typename T>
    const T* as() const
    {
        return dynamic_cast<const T*>(this);
    }

   /**
    * Converts Map_value to a json object.
    * Pure virtual method will be implemented by derived classes.
    * @param Writer object. Defines how to write to json.
    */
    virtual void to_json(Writer<OStreamWrapper>& writer) const = 0;

   /**
    * Converts Map_value to a xml object.
    * Pure virtual method will be implemented by derived classes.
    * @param xml_node object is filled with data of Map_value.
    */
    virtual void to_xml(pugi::xml_node& node) const = 0;

   /**
    * Converts Map_value to a yaml object.
    * Pure virtual method will be implemented by derived classes.
    * @param yaml emitter object is filled with data of Map_value.
    */
    virtual void to_yaml(YAML::Emitter& node) const = 0;

   /**
    * Writes the Map_value to the given stream.
    * Pure virtual method will be implemented in derived classes.
    * @param ostream& defines where to write.
    * @param Output_fromat in which format should the output appear.
    * @param Boolean quoted should the output be quoted or not.
    * @return Returns the stream being written to.
    * @see Output_format
    */
    virtual std::ostream& write(std::ostream& os, const Output_format format, bool quouted) const = 0;

};

/**
 * Value factory function. Create a unique_ptr of a value of type T instantiated with Args... .
 * @tparam T The type of the value that should be created.
 * @tparam Args The variadic template argument types.
 * @param args The arguments passed to the constructor of the value type T.
 * @return Returns a unique_ptr to the newly create value type object.
 */
template<typename T, typename ...Args>
std::unique_ptr<T> make_value(Args&& ...args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

} /* closing namespace adafs */

#endif /* __ADAFS_VALUE_HPP__ */
