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
    }

    void export_xml(const std::string& xmlfilename)
    {
        hwloc_topology_export_xml(this->_topology, xmlfilename.c_str());
    }

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
    void add_to_topology(const Device& dev) {};


private:
    hwloc_topology_t _topology;

};


} /* closing namesapce adafs */

#endif /* __ADAFS_SYSMAP_HPP__ */
