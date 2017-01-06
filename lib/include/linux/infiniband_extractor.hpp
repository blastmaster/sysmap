#ifndef __ADAFS_LINUX_INFINIBAND_EXTRACTOR_HPP__
#define __ADAFS_LINUX_INFINIBAND_EXTRACTOR_HPP__

#include "../extractors/infiniband_extractor.hpp"
#include <regex>

#undef linux

namespace adafs { namespace linux {

    struct Infiniband_Extractor : extractor::Infiniband_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<Infiniband_Extractor>(); }

        protected:
            virtual data collect() override;

        private:
            void collect_ibnetdiscover(data& result);
            std::string assemble_switch(const std::smatch& matches, data& result_graph);
            std::string assemble_host(const std::smatch& matches, data& result_graph);
            void assemble_connection(const std::smatch& matches, data& result_graph, const Peer_Type type, const std::string& from_guid);
    };

}} /* closing namespace adafs::linux */

#endif /* __ADAFS_LINUX_INFINIBAND_EXTRACTOR_HPP__ */
