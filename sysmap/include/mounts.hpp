#ifndef __ADAFS_MOUNTS_HPP__
#define __ADAFS_MOUNTS_HPP__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <mntent.h>

#include "utils.hpp"


namespace adafs {

/**
 * structure representing a mount entry.
 * this is one line in mtab/fstab/mountinfo
 */
struct mount_entry
{
    std::string device_name; // ????
    std::string mountpoint;
    std::string fs_type;
    std::vector<std::string> mount_opts;
    int dump_freq;
    int pass_number;


    mount_entry(struct mntent mnt) :
        device_name(mnt.mnt_fsname),
        mountpoint(mnt.mnt_dir),
        fs_type(mnt.mnt_type),
        mount_opts(utils::split(mnt.mnt_opts, ",")),
        dump_freq(mnt.mnt_freq),
        pass_number(mnt.mnt_passno) {}


    //FIXME this does not work!
    //because of static istringstream in split_first this will only work once!
    mount_entry(const std::string& str) :
        device_name(utils::split_first(str)),
        mountpoint(utils::split_first(str)),
        fs_type(utils::split_first(str)),
        mount_opts(utils::split(utils::split_first(str), ",")),
        dump_freq(std::stoi(utils::split_first(str))),
        pass_number(std::stoi(utils::split_first(str))) {}


    //FIXME ugly solution but works
    //here we try to solve the above probleme with a generator like approach
    //unfortunately we cannot pass the argument as const, while the next method
    //changes the members!
    mount_entry(utils::splitter& s) :
        device_name(s.next()),
        mountpoint(s.next()),
        fs_type(s.next()),
        mount_opts(utils::split(s.next(), ",")),
        dump_freq(std::stoi(s.next())),
        pass_number(std::stoi(s.next())) {}


    bool operator==(const mount_entry& m)
    {
        return (this->device_name == m.device_name &&
                this->mountpoint == m.mountpoint &&
                this->fs_type == m.fs_type &&
                this->mount_opts == m.mount_opts &&
                this->dump_freq == m.dump_freq &&
                this->pass_number == m.pass_number);
    }

    bool operator!=(const mount_entry& m)
    {
        return !(*this == m);
    }

};

// just for debugging
std::ostream& operator<< (std::ostream& os, const mount_entry& mntent)
{
    os << "Name of mounted filesystem: " << mntent.device_name << "\n";
    os << "Filesytem path prefix: " << mntent.mountpoint << "\n";
    os << "Mount type: " << mntent.fs_type << "\n";
    os << "Mount options:\n";
    for (auto& op : mntent.mount_opts) {
        os << "\t" << op << "\n";
    }
    os << "Dump freq. in days: " << mntent.dump_freq << "\n";
    os << "Pass number: " << mntent.pass_number << "\n";

    return os;
}


//FIXME
static
std::vector<mount_entry> load_mounts(const std::string& fname)
{
    std::ifstream in(fname);
    std::vector<mount_entry> v_me;

    if (!in) {
        std::cerr << "Error cannot open file stream\n";
        return v_me;
    }

    for (std::string line; getline(in, line);) {
        auto sp = utils::splitter(line);
        v_me.push_back(mount_entry(sp));
        //v_me.push_back(mount_entry(line));
    }

    in.close();

    return v_me;
}


static
std::vector<mount_entry> load_mount_with_getmntent(const std::string& fname)
{
    FILE *f = setmntent(fname.c_str(), "r");
    struct mntent* mnt;
    std::vector<mount_entry> v_me;

    while (mnt = getmntent(f)) {
        v_me.push_back(mount_entry(*mnt));
    }

    endmntent(f);
    return v_me;
}

} /* closing namespace adafs */

#endif /* __ADAFS_MOUNTS_HPP__ */
