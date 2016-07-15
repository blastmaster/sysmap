#include <iostream>

#include "sysmap.hpp"

using namespace adafs;


int main(int argc, char** argv)
{
    Sysmap smap {};
    auto block_devs = smap.block_devices();
    if (block_devs.empty()) {
        std::cerr << "Warning no block devices found!\n";
    }
    else {
        for (const auto& d : block_devs) {
            smap.add_to_topology(d);
        }
    }

    smap.export_xml("adafs_test.xml");

    return 0;
}
