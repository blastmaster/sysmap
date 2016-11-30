#include <iostream>
#include "extractor_set.hpp"
#include "linux/filesystem_extractor.hpp"
#include "linux/disk_extractor.hpp"
#include "linux/infiniband_extractor.hpp"
#include "hwloc/hwloc_extractor.hpp"
#include "output.hpp"


using namespace adafs;
using namespace std;

int main(int argc, char** argv)
{
    Extractor_Set extrset {};
    extrset.add_extractor("Filesystem", std::make_shared<linux::Filesystem_Extractor>());
    extrset.add_extractor("Disks", std::make_shared<linux::Disk_Extractor>());
    extrset.add_extractor("hwloc", std::make_shared<hwloc::Hwloc_Extractor>());
    extrset.add_extractor("infiniband", std::make_shared<linux::Infiniband_Extractor>());

    // extract data
    extrset.extract();

    // write to stdout as json
    extrset.write(cout, Output_format::JSON);

    return 0;
}
