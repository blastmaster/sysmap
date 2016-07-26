#ifndef __ADAFS_FILESYSTEM_EXTRACTOR_HPP__
#define __ADAFS_FILESYSTEM_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"

#include <vector>
#include <boost/filesystem.hpp>


namespace adafs { namespace extractor {

namespace fs = boost::filesystem;

using Sector = unsigned long long;
using Byte = unsigned long long;


struct Filesystem_Extractor  : Extractor {

    Filesystem_Extractor() {}

    virtual void load(Extractor_Set& findings) override;

protected:

    struct Mountpoint {

        Mountpoint() : size{0}, available{0} {}

        // device of the mountpoint
        std::string device;
        // name of the mountpoint (location)
        std::string mount;
        // filesystem of the mountpoint
        std::string filesystem;
        // options of the mountpoint
        std::vector<std::string> options;
        // size of the mountpoint
        uint64_t size;
        // available size of the mountpoint
        uint64_t available;

    };

    struct Partition {

            Partition() : major_device_number{0}, minor_device_number{0}, size{0}
            {}

            //TODO: is this constant? /sys/dev/block/sda/queue/hw_sector_size
            // need device path _not_ partition path!
            //sector_size = 512;
            //sector_end = sector_start + size - 1;

            // name of the partition
            std::string name;
            // TODO: major and minor device parsing
            // major number of device
            unsigned int major_device_number;
            // minor number of device
            unsigned int minor_device_number;
            // flag indicating if whole device is used by this partition;
            //bool whole_device;
            // uuid of ?
            std::string uuid;
            // uuid of partition
            std::string partition_uuid;
            // label of partition
            std::string label;
            // where is the partition mounted
            std::string mountpoint;
            // number of sectors
            Sector size;
            // sector where partition starts
            //Sector sector_start;
            // sector where partition ends
            //Sector sector_end;
            //TODO:
            //Sector sectors_used;    // number of sectors used within this partition
            //Sector sectors_unused;  // number of sectors unused within this partition
            // size of one sector in bytes
            //Byte sector_size;
    };

    struct data {

        std::vector<Partition> partitions;

        std::vector<Mountpoint> mountpoints;

    };

    virtual data collect() = 0;
};

}} /* closing namespace adafs::extractor */

#endif /* __ADAFS_FILESYSTEM_EXTRACTOR_HPP__ */
