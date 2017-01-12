#include "scalar_value.hpp"

using namespace rapidjson;

namespace adafs {

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

}
