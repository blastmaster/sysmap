#include <iostream>
#include "extractor_set.hpp"
#include "linux/filesystem_extractor.hpp"
#include "linux/disk_extractor.hpp"
#include "linux/infiniband_extractor.hpp"
#include "linux/kernel_extractor.hpp"
#include "hwloc/hwloc_machine_info_extractor.hpp"
#include "hwloc/hwloc_memory_info_extractor.hpp"
#include "hwloc/hwloc_pci_device_extractor.hpp"
#include "output.hpp"
#include "pugixml.hpp"

using namespace sysmap;
using namespace std;

int main()
{
    Extractor_Set extrset {};

    extrset.add_extractor("Memory", std::make_shared<hwloc::Hwloc_Memory_Info_Extractor>());
    extrset.add_extractor("Filesystem", std::make_shared<linux::Filesystem_Extractor>());
    extrset.add_extractor("Disks", std::make_shared<linux::Disk_Extractor>());
    extrset.add_extractor("PCI", std::make_shared<hwloc::Hwloc_PCI_Device_Extractor>());
    extrset.add_extractor("Machine", std::make_shared<hwloc::Hwloc_Machine_Info_Extractor>());
    extrset.add_extractor("Kernel", std::make_shared<linux::Kernel_Extractor>());
    extrset.add_extractor("Infiniband", std::make_shared<linux::Infiniband_Extractor>());
    
    // extract data
    extrset.extract();

    // write to stdout as json
    extrset.write(cout, Output_format::XML);

    return 0;
}

// vim:et
