#ifndef __SYSMAP_EXTRACTOR_SWAP_EXTRACTOR_HPP__
#define __SYSMAP_EXTRACTOR_SWAP_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"

namespace sysmap { namespace extractor {

    struct Swap_Extractor : Extractor {

        Swap_Extractor();

        virtual void load(Extractor_Set& findings) override;
        virtual void store(Extractor_Set& findings, const std::string& dbname) override;

        protected:

        struct SwapInfo
        {
            std::string filename;
            // type may be an enum (see man /proc/swaps)
            std::string type;
            std::uint64_t size;
            std::uint64_t used;
        };

        struct data
        {
            SwapInfo info;
        };

        virtual data collect() = 0;
    };

}} /* closing namespace sysmap::extractor */

#endif /* __SYSMAP_EXTRACTOR_SWAP_EXTRACTOR_HPP__ */
