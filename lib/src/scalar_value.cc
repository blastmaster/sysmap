#include "scalar_value.hpp"
#include "pugixml.hpp"

using namespace rapidjson;

namespace adafs {

    //JSON
    template<>
    void String_value::to_json(Writer<OStreamWrapper>& writer) const
    {
        writer.String(m_value.c_str());
    }

    template<>
    void Uint_value::to_json(Writer<OStreamWrapper>& writer) const
    {
        writer.Uint(m_value);
    }

    template<>
    void Int_value::to_json(Writer<OStreamWrapper>& writer) const
    {
        writer.Int(m_value);
    }

    template<>
    void Double_value::to_json(Writer<OStreamWrapper>& writer) const
    {
        writer.Double(m_value);
    }

    //XML
    template<>
    void String_value::to_xml(pugi::xml_node& node) const
    {
        node.append_attribute("String_value") = m_value;
    }

    template<>
    void Uint_value::to_xml(pugi::xml_node& node) const
    {
        node.append_attribute("Uint_value") = m_value;
    }

    template<>
    void Int_value::to_xml(pugi::xml_node& node) const
    {
        node.append_attribute("Int_value") = m_value;
    }

    template<>
    void Double_value::to_xml(pugi::xml_node& node) const
    {
        node.append_attribute("Double_value") = m_value;
    }
}
