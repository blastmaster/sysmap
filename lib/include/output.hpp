#ifndef __ADAFS_OUTPUT_HPP__
#define __ADAFS_OUTPUT_HPP__

#include <iostream>
#include <fstream>
#include <string>

namespace adafs {

    enum class Output_format
    {
        XML,
        ASCII,
        JSON,
    };

    struct outwrapper
    {
        outwrapper() = default;

        outwrapper(const std::string& name) : fout_(name) {}

        outwrapper(outwrapper& other) = delete;

        outwrapper operator=(outwrapper& other) = delete;

        void set_file(const std::string& name)
        {
            if (fout_.is_open()) {
                fout_.close();
            }
            fout_.open(name);
        }

        std::ostream& get_stream()
        {
            if (!fout_.is_open()) {
                return std::cout;
            }
            return fout_;
        }

        operator std::ostream& ()
        {
            return get_stream();
        }

        private:
            std::ofstream fout_;
    };

    struct Value; //forward declaration

    struct XML_Writer {

        static constexpr const char *xml_preamble = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" ;

        static constexpr const char *xml_root_element_open = "<system-topology>";

        static constexpr const char *xml_root_element_close = "</system-topology>";

        static void make_tag(std::ostream& os, const std::string& tagname, const Value* value);

        XML_Writer(std::ostream& oss);

        ~XML_Writer();

        void write_xml_attribute(const std::string& key, const Value* value);

        std::ostream& os;

    };

} /* closing namespace adafs */

#endif /* __ADAFS_OUTPUT_HPP__ */
