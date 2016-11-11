#ifndef __ADAFS_HWLOC_EXTRACTOR_HPP__
#define __ADAFS_HWLOC_EXTRACTOR_HPP__

#include "../extractors/hwloc_extractor.hpp"


namespace adafs { namespace hwloc {

    struct Hwloc_Extractor : extractor::Hwloc_Extractor {

        protected:

            virtual data collect() override;

        private:

            void collect_pci_devices(data& result);
            void collect_machine_info(data& result);
            void collect_memory_info(data& result);
            PCI_Device assemble_pci_device(const std::string& name,
                    const hwloc_obj_t& obj) const;

    };

}} /* closing namespace adafs::hwloc */

#endif /* __ADAFS_HWLOC_EXTRACTOR_HPP__ */
