
#include "mounts.hpp"

namespace adafs {

bool Mount_entry::mount_entry_cache_loaded = false;
std::vector<Mount_entry> Mount_entry::mount_entry_cache;


void Mount_entry::load_mounts(const std::string& fname)
{
    if (mount_entry_cache_loaded) {
        return;
    }

    mount_entry_cache.clear();
    FILE *f = setmntent(fname.c_str(), "r");
    struct mntent* mnt;

    while (mnt = getmntent(f)) {
        mount_entry_cache.emplace_back(*mnt);
    }

    endmntent(f);
    if (!mount_entry_cache.empty()) {
        mount_entry_cache_loaded = true;
    }
}


} /* closing namespace adafs */
