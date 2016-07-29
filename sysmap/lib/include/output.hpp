#ifndef __ADAFS_OUTPUT_HPP__
#define __ADAFS_OUTPUT_HPP__

#include <iostream>
#include <string>

namespace adafs {

    enum class Output_format {
        XML,
        ASCII,
    };

    using namespace std::string_literals;

    struct XMLOutputFromatter {

        XMLOutputFromatter() : m_preamble { "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"s } {}

        template<typename T>
        std::ostream& add_tag(std::ostream& os, const T& value) const
        {
            os << "<" << value << ">\n";
            return os;
        }

        private:
        std::string m_preamble;
    };

} /* closing namespace adafs */

#endif /* __ADAFS_OUTPUT_HPP__ */
