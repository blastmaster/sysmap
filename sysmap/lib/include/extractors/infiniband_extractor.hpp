#ifndef __ADAFS_EXTRACTOR_INFINIBAND_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_INFINIBAND_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"

namespace adafs { namespace extractor {

struct Infiniband_Extractor : Extractor {

    Infiniband_Extractor();

    virtual void load(Extractor_Set& findings) override;

protected:

    struct data {

    };

    virtual data collect() = 0;

};

}} /* closing namespace adafs::extractor */

#endif /* __ADAFS_EXTRACTOR_INFINIBAND_EXTRACTOR_HPP__ */
