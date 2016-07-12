#ifndef __ADAFS_DEVICE_HPP__
#define __ADAFS_DEVICE_HPP__

#include <hwloc.h>

#include "partitions.hpp"

namespace adafs {

/**
 * @class Device
 * The Device class represents a physical device such as /dev/sda.
 */

class Device
{
    public:
        Device(const hwloc_obj_t hwobj) : _obj_ref{hwobj}, _device_name{hwobj->name}
        {
            _device_id = {hwloc_obj_get_info_by_name(hwobj, "LinuxDeviceID")};
            _device_path = {"/dev/" + _device_name};
            //TODO: initialize _partions
            //init cache
            //TODO: FILENAME
            Proc_Partition::load_partitions("../test_data/partitions/prometheuspartitions.txt");
            auto dname_sz = _device_name.size();
            for (const auto& x : Proc_Partition::proc_partition_cache) {
                if (x.second.partition_name.compare(0, dname_sz, _device_name) == 0) {
                    _partitions.emplace_back(x.second);
                }
            }
        }

        std::string device_name() const { return _device_name; }
        std::string device_path() const { return _device_path; }
        std::string device_id() const { return _device_id; }

    private:
        const hwloc_obj_t _obj_ref; // reference, where to insert information
        std::string _device_name;
        std::string _device_path;
        std::string _device_id;
        std::vector<Partition> _partitions;
};

} /* closing namespace adafs */

#endif /* __ADAFS_DEVICE_HPP__ */
