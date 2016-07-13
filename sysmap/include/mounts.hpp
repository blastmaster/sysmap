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
struct Mount_entry
{
    std::string device_name; // ????
    std::string mountpoint;
    std::string fs_type;
    std::vector<std::string> mount_opts;
    int dump_freq;
    int pass_number;


    Mount_entry(const struct mntent& mnt) :
        device_name(mnt.mnt_fsname),
        mountpoint(mnt.mnt_dir),
        fs_type(mnt.mnt_type),
        mount_opts(utils::split(mnt.mnt_opts, ",")),
        dump_freq(mnt.mnt_freq),
        pass_number(mnt.mnt_passno) {}



    //FIXME ugly solution but works
    //here we try to solve the above probleme with a generator like approach
    //unfortunately we cannot pass the argument as const, while the next method
    //changes the members!
    Mount_entry(utils::splitter& s) :
        device_name(s.next()),
        mountpoint(s.next()),
        fs_type(s.next()),
        mount_opts(utils::split(s.next(), ",")),
        dump_freq(std::stoi(s.next())),
        pass_number(std::stoi(s.next())) {}


    bool operator==(const Mount_entry& m)
    {
        return (this->device_name == m.device_name &&
                this->mountpoint == m.mountpoint &&
                this->fs_type == m.fs_type &&
                this->mount_opts == m.mount_opts &&
                this->dump_freq == m.dump_freq &&
                this->pass_number == m.pass_number);
    }


    bool operator!=(const Mount_entry& m)
    {
        return !(*this == m);
    }

    static bool mount_entry_cache_loaded;
    static std::vector<Mount_entry> mount_entry_cache;
    static void load_mounts(const std::string&);

};

// just for debugging
//std::ostream& operator<< (std::ostream& os, const Mount_entry& mntent)
//{
    //os << "Name of mounted filesystem: " << mntent.device_name << "\n";
    //os << "Filesytem path prefix: " << mntent.mountpoint << "\n";
    //os << "Mount type: " << mntent.fs_type << "\n";
    //os << "Mount options:\n";
    //for (auto& op : mntent.mount_opts) {
        //os << "\t" << op << "\n";
    //}
    //os << "Dump freq. in days: " << mntent.dump_freq << "\n";
    //os << "Pass number: " << mntent.pass_number << "\n";

    //return os;
//}


//FIXME
//static
//std::vector<Mount_entry> load_mounts(const std::string& fname)
//{
    //std::ifstream in(fname);
    //std::vector<mount_entry> v_me;

    //if (!in) {
        //std::cerr << "Error cannot open file stream\n";
        //return v_me;
    //}

    //for (std::string line; getline(in, line);) {
        //auto sp = utils::splitter(line);
        //v_me.emplace_back(sp);
    //}

    //in.close();

    //return v_me;
//}

//static
//std::vector<Mount_entry> load_mounts()
//{
    //return load_mounts("/proc/mounts");
//}



} /* closing namespace adafs */

#endif /* __ADAFS_MOUNTS_HPP__ */
