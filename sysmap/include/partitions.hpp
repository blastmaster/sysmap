#ifndef __ADAFS_PARTITIONS_HPP__
#define __ADAFS_PARTITIONS_HPP__

#include <iostream>
#include <fstream>
#include <vector>
#include <boost/filesystem.hpp>

#include "proc_partitions.hpp"
#include "mounts.hpp"

namespace adafs {

namespace fs = boost::filesystem;

using Sector = unsigned long long;
using Byte = unsigned long long;

static std::string read_sysfs_file(const std::string& name)
{
    std::ifstream in {name};
    std::string buf;
    getline(in, buf);
    return buf;
}

static std::string read_sysfs_file(const fs::path& p)
{
    return read_sysfs_file(p.string());
}

static fs::path build_partition_path(const std::string& device_id)
{
    fs::path sysdev {"/sys/dev/block"};
    return sysdev / device_id;
}

static fs::path build_partition_path(int major_id, int minor_id)
{
    std::string s_devid = std::to_string(major_id) + ":" + std::to_string(minor_id);
    return build_partition_path(s_devid);
}

static std::string uuid_from_partition(const std::string& device_name)
{
    auto d_iter = fs::directory_iterator("/dev/disk/by-uuid/");
    auto found = std::find_if(fs::begin(d_iter), fs::end(d_iter),
            [&device_name](const fs::directory_entry& d_ent) {
                return fs::read_symlink(d_ent).filename().string() == device_name;
            });
    if (found == fs::end(d_iter)) {
        return "";
    }
    return (*found).path().filename().string();
}

class Partition {

    public:
        //TODO:
        //Partition(const std::string& dev_path) : device_path{dev_path} {}

        Partition(const Proc_Partition& pp) : device_path{pp.device_path()},
                                              device_number{pp.major_dev_num},
                                              partition_number{pp.minor_dev_num},
                                              whole_device{false}
        {
            //TODO: is this constant? /sys/dev/block/sda/queue/hw_sector_size
            // need device path _not_ partition path!
            sector_size = 512;
            auto device_dir = build_partition_path(device_number, partition_number);
            fs::path size_file = device_dir / "size";
            fs::path start_file = device_dir / "start";
            size = stoull(read_sysfs_file(size_file));
            //TODO: the whole device has no start file!
            //here the exception is thrown by stoull.
            //can we catch them before? sector_start = 0 should be valid.
            try {
                sector_start = stoull(read_sysfs_file(start_file));
            } catch (std::invalid_argument& e) {
                std::cerr << e.what() << "\n";
                sector_start = 0;
            }
            sector_end = sector_start + size - 1;
            //TODO: passing just partition name like "sda1" or path "/dev/sda1"
            //but then as fs::path?
            uuid = uuid_from_partition(device_path.filename().string());
            //TODO: initialize mountpoints
            for (const auto& m : Mount_entry::mount_entry_cache) {
                if (m.device_name == device_path.string()) {
                    _mountpoints.push_back(m);
                }
            }
        }

        const std::vector<Mount_entry>& get_mounts() const { return _mountpoints; }

        fs::path device_path;
        int device_number;
        int partition_number;
        bool whole_device;      // flag indicating if whole device is used by this partition;
        //TODO:
        //Filesystem file_system; // filesytem used on partion
        std::string uuid;       // uuid of partition
        Sector size;            // number of sectors
        Sector sector_start;    // sector where partition starts
        Sector sector_end;      // sector where partition ends
        //TODO:
        //Sector sectors_used;    // number of sectors used within this partition
        //Sector sectors_unused;  // number of sectors unused within this partition
        Byte sector_size;       // size of one sector in bytes

    private:
        std::vector<Mount_entry> _mountpoints;
};

} /* closing namespace adafs */

#endif /* __ADAFS_PARTITIONS_HPP__ */
