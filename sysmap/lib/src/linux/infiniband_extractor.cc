#include "linux/infiniband_extractor.hpp"
#include "utils.hpp"


namespace adafs { namespace linux {

    Infiniband_Extractor::data Infiniband_Extractor::collect()
    {
        data result;
        collect_ibnetdiscover(result);
        return result;
    }

    void Infiniband_Extractor::collect_ibnetdiscover(data& result)
    {
        // TODO:
    }

}} /* closing namespace adafs::linux */
