#include "linux/filesystem_extractor.hpp"
#include "utils/file.hpp"

#include <sys/vfs.h>
#include <mntent.h>

#include <map>
#include <algorithm>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

namespace adafs { namespace linux {

    std::string Filesystem_Extractor::info_from_dev_disk(const std::string& partname, const std::string& what)
    {
        fs::path path {"/dev/disk/"};
        auto d_iter = fs::directory_iterator {path / what};
        auto found = std::find_if(fs::begin(d_iter), fs::end(d_iter),
                [&partname](const fs::directory_entry& d_ent) {
                    return fs::read_symlink(d_ent).filename().string() == partname;
                });
        if (found == fs::end(d_iter)) {
            return "";
        }
        return (*found).path().filename().string();
    }

    Filesystem_Extractor::data Filesystem_Extractor::collect()
    {
        data result;
        collect_mountpoints(result);
        collect_partitions(result);
        return result;
    }

    void Filesystem_Extractor::collect_mountpoints(data& result)
    {
        FILE *file = setmntent("/proc/self/mounts", "r");
        struct mntent entry;
        char buffer[4096];

        while (struct mntent *mnt_ptr = getmntent_r(file, &entry, buffer, sizeof(buffer))) {
            std::string device = mnt_ptr->mnt_fsname;

            // skip anything that doesn't map to a device
            // can we do this? what is with fuse?
            if (! boost::starts_with(device, "/dev/")) {
                continue;
            }

            Mountpoint mntpnt;
            mntpnt.device = std::move(device);
            mntpnt.mount = mnt_ptr->mnt_dir;
            mntpnt.filesystem = mnt_ptr->mnt_type;
            mntpnt.options = mnt_ptr->mnt_opts;

            struct statfs stat_info;
            if (statfs(mnt_ptr->mnt_dir, &stat_info) != -1) {
                // size of the filesystem is fragment size * number of blocks
                mntpnt.size = stat_info.f_frsize * stat_info.f_blocks;
                mntpnt.available = stat_info.f_frsize * stat_info.f_bfree;
            }

            result.mountpoints.emplace_back(std::move(mntpnt));
        }

        endmntent(file);
        file = nullptr;
    }

    void Filesystem_Extractor::collect_partitions(data& result)
    {
        std::map<std::string, std::string> mntpnts;
        for (const auto& point : result.mountpoints) {
            mntpnts.insert(std::make_pair(point.device, point.mount));
        }

        utils::file::for_each_subdirectory("/sys/block/", [&](const std::string& subdir) {
                fs::path block_device_path {subdir};
                auto block_device_filename = block_device_path.filename().string();

                if (fs::is_directory(block_device_path / "device")) { // is whole disk device!

                    utils::file::for_each_subdirectory(subdir, [&](const std::string& subdir) {
                            fs::path partition_path {subdir};
                            auto partition_name = partition_path.filename().string();

                            if (! boost::starts_with(partition_name, block_device_filename)) {
                                return true;
                            }
                            Partition part;
                            part.name = "/dev/" + partition_name;
                            collect_partition_attributes(part, mntpnts, subdir, partition_name);
                            result.partitions.emplace_back(std::move(part));
                            return true;
                    });
                }
                else if (fs::is_directory(block_device_path / "dm")) { // is dev-mapper device

                    auto mapping_name = utils::file::read((block_device_path / "dm" / "name").string());
                    boost::trim(mapping_name);
                    if (mapping_name.empty()) {
                        mapping_name = "/dev/" + block_device_filename;
                    }
                    else {
                        mapping_name = "/dev/mapper/" + mapping_name;
                    }
                    Partition part;
                    part.name = std::move(mapping_name);
                    collect_partition_attributes(part, mntpnts, block_device_path.string(), block_device_filename);
                    result.partitions.emplace_back(std::move(part));
                }
                //TODO: what is with loop devices?
                return true;
        });
    }

    void Filesystem_Extractor::collect_partition_attributes(Partition& part, const std::map<std::string, std::string>& mntpnts,
            const std::string& partition_path, const std::string& partition_name)
    {
        part.size = std::stoull(utils::file::read(partition_path + "/size"));
        part.device_number = utils::file::read(partition_path + "/dev");
        boost::trim(part.device_number);

        part.uuid = info_from_dev_disk(partition_name, "by-uuid");
        part.partition_uuid = info_from_dev_disk(partition_name, "by-partuuid");
        part.label = info_from_dev_disk(partition_name, "by-label");

        auto iter = mntpnts.find(part.name);
        if (iter != mntpnts.end()) {
            part.mountpoint = iter->second;
        }
    }

}} /* closing namesapce adafs::linux */
