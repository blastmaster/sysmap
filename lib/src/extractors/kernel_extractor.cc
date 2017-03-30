#include "value.hpp"
#include "scalar_value.hpp"
#include "array_value.hpp"
#include "map_value.hpp"
#include "utils.hpp"

#include "extractors/kernel_extractor.hpp"

namespace adafs { namespace extractor {

    void Kernel_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();
    }

    void Kernel_Extractor::store(Extractor_Set& findings, const std::string& dbname)
    {
    }
} }

