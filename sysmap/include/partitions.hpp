#ifndef __ADAFS_PARTITIONS_HPP__
#define __ADAFS_PARTITIONS_HPP__

#include <iostream>
#include <fstream>
#include <vector>

#include "mounts.hpp"

namespace adafs {

using Sector = unsigned long long;
using Byte = unsigned long long;

static std::string read_sysfs_file(const std::string& name)
{
    std::ifstream in {name};
    std::string buf;
    getline(in, buf);
    return buf;
}

class Partition {

    public:
        Partition(const std::string& dev_path) : device_path{dev_path} {}

        Partition(const Proc_Partition& pp) : device_path{pp.dev_path()},
                                              partition_number{pp.minor_dev_num},
                                              device_number{pp.major_dev_num},
                                              whole_device{false}
        {
            //TODO: init sizes
            static std::string root_sysdev {"/sys/dev/block/"};
            sector_size = 512; // is this constant? queue/hw_sector_size
            std::string device_dir = root_sysdev + std::to_string(device_number) +
                ":" + std::to_string(partition_number) + "/";

            std::string size_file = device_dir + "size";
            std::string start_file = device_dir + "start";

            size = stoull(read_sysfs_file(size_file));
            sector_start = stoull(read_sysfs_file(start_file));
            sector_end = sector_start + size + 1;
            //TODO: initialize mountpoints

        }

        std::string device_path;
        int device_number;
        int partition_number;
        bool whole_device;      // flag indicating if whole device is used by this partition;
        //TODO:
        //Filesystem file_system; // filesytem used on partion
        //TODO: needs libuuid?
        //std::string uuid;       // uuid of partition
        //std::string label;
        Sector size;            // number of sectors
        Sector sector_start;    // sector where partition starts
        Sector sector_end;      // sector where partition ends
        //TODO:
        //Sector sectors_used;    // number of sectors used within this partition
        //Sector sectors_unused;  // number of sectors unused within this partition
        Byte sector_size;       // size of one sector in bytes

    private:
        std::vector<mount_entry> _mountpoints;
};

} /* closing namespace adafs */

#endif /* __ADAFS_PARTITIONS_HPP__ */
