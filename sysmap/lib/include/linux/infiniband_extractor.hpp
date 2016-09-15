#ifndef __ADAFS_LINUX_INFINIBAND_EXTRACTOR_HPP__
#define __ADAFS_LINUX_INFINIBAND_EXTRACTOR_HPP__

#include "../extractors/infiniband_extractor.hpp"

namespace adafs { namespace linux {

    struct Infiniband_Extractor : extractor::Infiniband_Extractor {

        protected:
            virtual data collect() override;

        private:
            void collect_ibnetdiscover(data& result);

    };

}} /* closing namespace adafs::linux */

#endif /* __ADAFS_LINUX_INFINIBAND_EXTRACTOR_HPP__ */
