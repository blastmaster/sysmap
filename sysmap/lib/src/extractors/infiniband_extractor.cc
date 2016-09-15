#include "value.hpp"
#include "scalar_value.hpp"
#include "map_value.hpp"
#include "array_value.hpp"

#include "extractors/infiniband_extractor.hpp"

namespace adafs { namespace extractor {

    Infiniband_Extractor::Infiniband_Extractor() : Extractor("Infiniband") {}

    void Infiniband_Extractor::load(Extractor_Set& findings)
    {
        // TODO
    }

}} /* closing namespace adafs::extractor */
