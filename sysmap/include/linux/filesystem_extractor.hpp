#ifndef __ADAFS_LINUX_FILESYSTEM_EXTRACTOR_HPP__
#define __ADAFS_LINUX_FILESYSTEM_EXTRACTOR_HPP__

namespace adafs {  namespace linux {

    struct Filesystem_Extractor : extractor::Filesystem_Extractor {

        static std::string info_from_dev_disk(const std::string& partname, const std::string& what);

        protected:

            virtual data collect() override;

        private:

            void collect_mountpoints(data& result);
            void collect_partitions(data& result);
            void collect_partition_attributes(Partition& part, const std::map<std::string, std::string>& mntpnts,
                    const std::string& partition_path, const std::string& partition_name);
    };

}} /* closing namespace adafs::extractor::linux */

#endif /* __ADAFS_LINUX_FILESYSTEM_EXTRACTOR_HPP__ */
