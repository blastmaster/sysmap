#include "output.hpp"
#include "value.hpp"
#include "pugixml.hpp"


namespace adafs {


    void
    XML_Writer::make_tag(pugi::xml_document& doc, const std::string& tagname, const Value* value)
    {
        value->write(doc, Output_format::XML, true);
    }

    XML_Writer::XML_Writer(std::ostream& oss) : os{oss} 
    {
        os << "<object";
    }

    XML_Writer::~XML_Writer()
    {
        os << "/>\n";
    }

    void XML_Writer::write_xml_attribute(const std::string& key, const Value* value)
    {
        os << " " << key << "=";
        value->write(os, Output_format::XML, true);
    }


} /* closing namespace adafs */
