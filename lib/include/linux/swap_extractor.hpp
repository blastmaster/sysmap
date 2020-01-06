#ifndef __SYSMAP_SWAP_EXTRACTOR_HPP__
#define __SYSMAP_SWAP_EXTRACTOR_HPP__

#include "../extractors/swap_extractor.hpp"

namespace sysmap { namespace linux {

    struct Swap_Extractor : extractor::Swap_Extractor
    {
        static std::unique_ptr<Extractor> create()
        {
            return std::make_unique<Swap_Extractor>();
        }

        protected:
        virtual data collect() override;

        private:
        static Registrar registrar;
        void collect_swap_info_data(data& result);
    };

}} /* closing namespace sysmap::linux */

#endif /* __SYSMAP_SWAP_EXTRACTOR_HPP__ */
