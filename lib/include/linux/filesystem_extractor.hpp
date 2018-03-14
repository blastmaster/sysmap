#ifndef __SYSMAP_LINUX_FILESYSTEM_EXTRACTOR_HPP__
#define __SYSMAP_LINUX_FILESYSTEM_EXTRACTOR_HPP__

#include "../extractors/filesystem_extractor.hpp"

#undef linux

namespace sysmap {  namespace linux {

    struct Filesystem_Extractor : extractor::Filesystem_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<linux::Filesystem_Extractor>(); }
        static std::string info_from_dev_disk(const std::string& partname, const std::string& what);

        protected:
            virtual data collect() override;

        private:
            static Registrar registrar;
            void collect_mountpoints(data& result);
            void collect_partitions(data& result);
            void collect_partition_attributes(Partition& part, const std::map<std::string, std::string>& mntpnts,
                    const std::string& partition_path, const std::string& partition_name);
    };

}} /* closing namespace sysmap::linux */

#endif /* __SYSMAP_LINUX_FILESYSTEM_EXTRACTOR_HPP__ */
