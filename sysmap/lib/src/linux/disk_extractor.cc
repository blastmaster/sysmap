#include "linux/disk_extractor.hpp"
#include "utils/file.hpp"

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
        utils::file::for_each_subdirectory("/sys/block/", [&](const std::string& subdir) {

                    fs::path block_device_directory {subdir};
                    Disk d;
                    d.name = block_device_directory.filename().string();
                    fs::path block_device_subdirectory = block_device_directory / "device";

                    if (!fs::is_directory(block_device_subdirectory)) {
                        return true;
                    }

                    auto devnum_file = (block_device_directory / "dev").string();
                    auto size_file = (block_device_directory / "size").string();
                    auto model_file = (block_device_subdirectory / "model").string();
                    auto vendor_file = (block_device_subdirectory / "vendor").string();

                    // vendor
                    d.vendor = utils::file::read(vendor_file);
                    boost::trim(d.vendor);
                    // model
                    d.model = utils::file::read(model_file);
                    boost::trim(d.model);
                    // size
                    auto size = utils::file::read(size_file);
                    boost::trim(size);
                    d.size = std::stoull(size);
                    // device number
                    d.device_number = utils::file::read(devnum_file);
                    boost::trim(d.device_number);

                    result.disks.emplace_back(std::move(d));
                    return true;
                });
    }

}} /* closing namespace adafs::linux */
