#include <iostream>

#include "extractor_set.hpp"
#include "linux/filesystem_extractor.hpp"
#include "linux/disk_extractor.hpp"
#include "hwloc/hwloc_extractor.hpp"
#include "output.hpp"

using namespace adafs;


int main(int argc, char** argv)
{

    Extractor_Set extr_set {};

    extr_set.add_extractor("Filesystem", std::make_shared<linux::Filesystem_Extractor>());
    extr_set.add_extractor("Disks", std::make_shared<linux::Disk_Extractor>());
    extr_set.add_extractor("hwloc", std::make_shared<hwloc::Hwloc_Extractor>());
    extr_set.extract();
    extr_set.write(std::cout, Output_format::XML);


    return 0;
}
