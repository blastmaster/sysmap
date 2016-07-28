#include "linux/disk_extractor.hpp"
#include "utils.hpp"

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

namespace adafs { namespace linux {

    Disk_Extractor::data Disk_Extractor::collect()
    {
        data result;
        collect_disk_data(result);
        return result;
    }

    void Disk_Extractor::collect_disk_data(data& result)
    {
        utils::for_each_subdirectory("/sys/block/", [&](const std::string& subdir) {

                    fs::path block_device_directory {subdir};
                    Disk d;
                    d.name = block_device_directory.filename().string();
                    fs::path block_device_subdirectory = block_device_directory / "device";

                    if (!fs::is_directory(block_device_subdirectory)) {
                        return true;
                    }

                    auto size_file = (block_device_directory / "size").string();
                    auto model_file = (block_device_subdirectory / "model").string();
                    auto vendor_file = (block_device_subdirectory / "vendor").string();

                    // vendor
                    d.vendor = utils::read(vendor_file);
                    boost::trim(d.vendor);
                    // model
                    d.model = utils::read(model_file);
                    boost::trim(d.model);
                    // size
                    auto size = utils::read(size_file);
                    boost::trim(size);
                    d.size = std::stoull(size);

                    result.disks.emplace_back(std::move(d));
                    return true;
                });
    }

}} /* closing namespace adafs::linux */
