#include <iostream>
#include <cstring>
#include "extractor_set.hpp"
#include "linux/infiniband_extractor.hpp"
#include "linux/filesystem_extractor.hpp"
#include "linux/disk_extractor.hpp"
#include "hwloc/hwloc_machine_info_extractor.hpp"
#include "hwloc/hwloc_memory_info_extractor.hpp"
#include "hwloc/hwloc_pci_device_extractor.hpp"
#include "utils.hpp"

#include <unistd.h>

using namespace std;
using namespace adafs;

/**
 * TODO
 * - create class maybe friend of infiniband extractor to get access to
 *   protected memeber classes.
 * - database could be class member so passing around is not necessary
 *
 */


static std::string get_hostname()
{
    char hname[256] = "";
    if (gethostname(hname, sizeof(hname)) != 0) {
        utils::log::logging::error() << "Could not determine Hostname: " << std::strerror(errno);
        return {};
    }
    return {hname};
}

int main(int argc, char** argv)
{

    Extractor_Set extr_set {};
    extr_set.add_extractor("Filesystem", std::make_shared<linux::Filesystem_Extractor>());
    extr_set.add_extractor("Disks", std::make_shared<linux::Disk_Extractor>());
    extr_set.add_extractor("PCI", std::make_shared<hwloc::Hwloc_PCI_Device_Extractor>());
    extr_set.add_extractor("Memory", std::make_shared<hwloc::Hwloc_Memory_Info_Extractor>());
    extr_set.add_extractor("Machine", std::make_shared<hwloc::Hwloc_Machine_Info_Extractor>());
    extr_set.add_extractor("Infiniband", std::make_shared<linux::Infiniband_Extractor>());
    extr_set.extract();
    auto hostname = get_hostname();
    std::string dbname = hostname + ".db";
    extr_set.save(dbname);

    return 0;
}
