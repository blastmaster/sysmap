#ifndef __ADAFS_SYSMAP_HPP__
#define __ADAFS_SYSMAP_HPP__

#include "device.hpp"
#include "mounts.hpp"
#include "partitions.hpp"

#include <hwloc.h>

namespace adafs {


static void load_topology(hwloc_topology_t* topo)
{
    hwloc_topology_init(topo);
    hwloc_topology_set_flags(*topo, HWLOC_TOPOLOGY_FLAG_WHOLE_IO);
    hwloc_topology_load(*topo);
}


class Sysmap
{

public:
    explicit Sysmap() 
    {
        load_topology(&_topology);
    }

    ~Sysmap()
    {
        hwloc_topology_destroy(_topology);
        //TODO: it seems that std::ifstream cannot work with virtual files like
        // /proc/partitions
        //Proc_Partition::load_partitions("test_data/partitions/prometheuspartitions.txt");
        Proc_Partition::load_partitions("/proc/partitions");
        Mount_entry::load_mounts("/proc/mounts");
    }

    void export_xml(const std::string& xmlfilename)
    {
        hwloc_topology_export_xml(this->_topology, xmlfilename.c_str());
    }

    // TODO: should be private? hwloc_ref of device obj gets invalidated if
    // sysmap object lifetimes end.
    // does this find loopback devices too? (test on taurus)
    std::vector<Device> block_devices() const
    {
        std::vector<Device> b_devs;
        int num_os_devs = hwloc_get_nbobjs_by_type(this->_topology, HWLOC_OBJ_OS_DEVICE);
        for (int i = 0; i < num_os_devs; ++i) {
            auto obj = hwloc_get_obj_by_type(this->_topology, HWLOC_OBJ_OS_DEVICE, i);
            if (!obj) {
                continue;
            }
            if (obj->attr->osdev.type == HWLOC_OBJ_OSDEV_BLOCK) {
                b_devs.emplace_back(obj);
            }
        }

        return b_devs;
    }

    //TODO: adding device to hwloc topology
    void add_to_topology(const Device& dev)
    {
        hwloc_obj_t parent = dev.get_hwloc_ref();
        // Adding a MISC object BlockDeviceInfo under the device
        hwloc_obj_t new_obj = hwloc_topology_insert_misc_object_by_parent(this->_topology,
                parent, "BlockDeviceInfo");

        if (!new_obj) {
            std::cerr << "error: creating BlockDeviceInfo object failed!\n";
            return;
        }
        for (auto& part : dev.get_partitions()) {
            hwloc_obj_add_info(new_obj, "DevicePath", part.device_path.string().c_str());
            hwloc_obj_add_info(new_obj, "UUID", part.uuid.c_str());
            hwloc_obj_add_info(new_obj, "Start", std::to_string(part.sector_start).c_str());
            hwloc_obj_add_info(new_obj, "End", std::to_string(part.sector_end).c_str());
            hwloc_obj_add_info(new_obj, "Size", std::to_string(part.size).c_str());
            // adding a MISC object Mounts under the BlockDeviceInfo
            hwloc_obj_t mnt_obj = hwloc_topology_insert_misc_object_by_parent(this->_topology,
                    new_obj, "Mountpoints");

            if (!mnt_obj) {
                std::cerr << "error: creating Mountpoints object failed!\n";
                return;
            }

            for (auto& mnt : part.get_mounts()) {
                hwloc_obj_add_info(mnt_obj, "DeviceName", mnt.device_name.c_str());
                hwloc_obj_add_info(mnt_obj, "MountPoint", mnt.mountpoint.c_str());
                hwloc_obj_add_info(mnt_obj, "FileSystem", mnt.fs_type.c_str());
                //TODO: add at least mount options
            }
        }
    }


private:
    hwloc_topology_t _topology;

};


} /* closing namesapce adafs */

#endif /* __ADAFS_SYSMAP_HPP__ */
