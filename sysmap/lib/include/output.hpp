#ifndef __ADAFS_OUTPUT_HPP__
#define __ADAFS_OUTPUT_HPP__

#include <iostream>
#include <string>

namespace adafs {

    enum class Output_format {
        XML,
        ASCII,
    };


    struct XMLOutput {

        static constexpr const char *xml_preamble = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" ;

        static constexpr const char *xml_root_element_open = "<system-topology>";

        static constexpr const char *xml_root_element_close = "</system-topology>";

    };

} /* closing namespace adafs */

#endif /* __ADAFS_OUTPUT_HPP__ */
