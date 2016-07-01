#ifndef __MOUNTS_HPP__
#define __MOUNTS_HPP__

#include <iostream>
#include <fstream>
#include <sstream>
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
void load_mounts(const std::string& fname)
{
    std::ifstream in(fname);

    if (!in) {
        std::cerr << "Error cannot open file stream\n";
        return;
    }

    for (std::string line; getline(in, line);) {
        std::vector<std::string> entry;
        entry = utils::split(line, " ", entry);
        for (auto &x : entry) {
            std::cout << "Entry: " << x << "\n";
        }
        std::cout << "--------------------\n";
    }

    in.close();

    return;
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

//int main()
//{
    //std::vector<std::string> test_files { "test_data/mounts/prometheusprocmounts.txt",
                                          //"test_data/mounts/prometheusmtab.txt",
                                          //"test_data/mounts/prometheusfstab.txt" };

    ////TODO
    ////load_mounts(filename);
    //for (auto& fn : test_files) {
        //auto v = load_mount_with_getmntent(fn);
        //std::cout << "================================================================================\n";
        //std::cout << fn << "\n\n";
        //for (auto& x : v) {
            //std::cout << x << "\n";
            //std::cout << "--------------------\n\n";
        //}
    //}

    //return 0;
//}

#endif /* __MOUNTS_HPP__ */
