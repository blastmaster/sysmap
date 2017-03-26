#ifndef __ADAFS_GET_MACHINE_INFO_EXTRACTOR_HPP__
#define __ADAFS_GET_MACHINE_INFO_EXTRACTOR_HPP__

#include "../extractors/machine_info_extractor.hpp"

#undef linux

namespace adafs { namespace linux {
    
    struct Get_Machine_Info_Extractor : extractor::Machine_Info_Extractor
    {

        static std::unique_ptr<Extractor> create() { return std::make_unique<Get_Machine_Info_Extractor>(); }
        
        Get_Machine_Info_Extractor() {}

        virtual ~Get_Machine_Info_Extractor() {}

        protected:
            virtual data collect() override;

        private:
            static Registrar registrar;
            void collect_machine_hostname(data& result);
            void collect_kernel_version(data& result);
            void collect_kernel_modules(data& result);
    };    


}} /* closing namespace adafs::hwloc */

#endif /* __ADAFS_GET_MACHINE_INFO_EXTRACTOR_HPP__*/
