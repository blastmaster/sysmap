#include "linux/lustre_extractor.hpp"
#include "utils.hpp"

#include <boost/format.hpp>
#include <cerrno>

enum lov_comp_md_flags {
        /* Replication states, use explained in replication HLD. */
        LCM_FL_RS_READ_ONLY     = 0,
        LCM_FL_RS_WRITE_PENDING = 1,
        LCM_FL_RS_WRITABLE      = 2,
        LCM_FL_RS_SYNC_PENDING  = 3,
        LCM_FL_RS_MASK          = 0xff,

        LCM_FL_PRIMARY_SET      = 1 << 15,
};

extern "C" {
#include "lustre/lustreapi.h"
#include <fcntl.h>    /* For O_RDONLY */
#include <unistd.h>   /* For open() */
#include <stdlib.h>
};

#define UUF     "%-20s"
#define CSF     "%11s"
#define CDF     "%11llu"
#define HDF     "%8.1f%c"
#define RSF     "%4s"
#define RDF     "%3d%%"

// Calculates ratio bettween used disk space and total disk space
int obd_statfs_ratio(const obd_statfs *stat_buf)
{
    double avail, used, ratio = 0;

    avail = stat_buf->os_bavail;
    used  = stat_buf->os_blocks - stat_buf->os_bfree;

    if (avail + used > 0) {
	ratio = used / (used + avail) * 100 + 0.5;
    }

    return static_cast<int>(ratio);
}


namespace sysmap { namespace linux {

    Registrar Lustre_Extractor::registrar("lustre", &linux::Lustre_Extractor::create);

    Lustre_Extractor::data Lustre_Extractor::collect()
    {
        data result;
        collect_mountpoint_data(result);
        return result;
    }

    int Lustre_Extractor::mntdf(char *mntdir, char *fsname,
    		 int ops, struct ll_statfs_buf *lsb,
                 mountpoint *mntpoint)
    {
        struct ll_stat_type {
   	    int   st_op;
   	    char *st_name;
        };

    	ll_stat_type types[] = {
            { LL_STATFS_LMV, "MDT" },
            { LL_STATFS_LOV, "OST" },
            { 0, nullptr }
        };

    	ll_stat_type *tp;

    	struct obd_statfs stat_buf;
        struct obd_statfs sum;
        sum.os_bsize = 1;

    	struct obd_uuid uuid_buf;
    	char *poolname = nullptr;
    	__u64 ost_ffree = 0;
    	__u32 index;
    	__u32 type;
    	int fd;
    	int rc = 0;
    	int rc2;

    	fd = open(mntdir, O_RDONLY);
    	if (fd < 0) {
    		rc = -errno;
    		fprintf(stderr, "sysmap: cannot open '%s': %s\n", mntdir,
    			strerror(errno));
    		return rc;
    	}

    	for (tp = types; tp->st_name != nullptr; tp++) {
    		if (!(tp->st_op & ops))
    			continue;

    		for (index = 0; ; index++) {
    			memset(&stat_buf, 0, sizeof(struct obd_statfs));
    			memset(&uuid_buf, 0, sizeof(struct obd_uuid));

    			type = tp->st_op;
    			rc2 = llapi_obd_fstatfs(fd, type, index,
    					       &stat_buf, &uuid_buf);
    			if (rc2 == -ENODEV)
    				break;
    			if (rc2 == -EAGAIN)
    				continue;
    			if (rc2 == -ENODATA) {
    					continue;
    			} else if (rc2 < 0 && rc == 0) {
    				rc = rc2;
    			}

    			if (poolname && tp->st_op == LL_STATFS_LOV &&
    			    llapi_search_ost(fsname, poolname,
    					     obd_uuid2str(&uuid_buf)) != 1)
    				continue;

    			if (uuid_buf.uuid[0] == '\0')
    				snprintf(uuid_buf.uuid, sizeof(uuid_buf.uuid),
    					 "%s%04x", tp->st_name, index);

			int shift = 10;
			long long avail = (stat_buf.os_bavail * stat_buf.os_bsize) >> shift;
			long long used  = ((stat_buf.os_blocks - stat_buf.os_bfree) *
				 stat_buf.os_bsize) >> shift;
			long long total = (stat_buf.os_blocks * stat_buf.os_bsize) >> shift;
                        auto uuid = obd_uuid2str(&uuid_buf);

	                char tbuf[3 * sizeof(__u64)];
	                char ubuf[3 * sizeof(__u64)];
	                char abuf[3 * sizeof(__u64)];
	                char rbuf[3 * sizeof(__u64)];
                        snprintf(tbuf, sizeof(tbuf), CDF, total);
			snprintf(ubuf, sizeof(ubuf), CDF, used);
			snprintf(abuf, sizeof(abuf), CDF, avail);

		        auto ratio_ = obd_statfs_ratio(&stat_buf);
		        sprintf(rbuf, RDF, ratio_);

                        boost::format fmt_uuid("%-20s");
                        boost::format fmt_size("%11s");
                        boost::format fmt_percentage("%4s");
                        mdt mdt_;

                        //set uuid
                        fmt_uuid % uuid;
                        mdt_.uuid = fmt_uuid.str();

                        //set used
                        fmt_size % ubuf;
                        mdt_.used = fmt_size.str();

                        //set avail
                        fmt_size % abuf;
                        mdt_.avail = fmt_size.str();

                        //set total
                        fmt_size % tbuf;
                        mdt_.blocks = fmt_size.str();

                        //set used percentage
                        fmt_percentage % rbuf;
                        mdt_.usage_percent = fmt_percentage.str();

                        //set mntdir[ost/mdt:index]
                        std::stringstream str;
                        str << mntdir << "[" << tp->st_name << ":" << index << "]";
                        mdt_.path =  str.str();
                        mdt_.index = index;

                        //TODO: find better method
                        if (tp->st_name[0] == 'M'){;
                            mntpoint->mdts.push_back(mdt_);
                        }
                        if (tp->st_name[0] == 'O'){
                            mntpoint->osts.push_back(mdt_);
                        }

    			if (rc2 == 0) {
    				if (tp->st_op == LL_STATFS_LMV) {
    					sum.os_ffree += stat_buf.os_ffree;
    					sum.os_files += stat_buf.os_files;
    				} else /* if (tp->st_op == LL_STATFS_LOV) */ {
    					sum.os_blocks += stat_buf.os_blocks *
    						stat_buf.os_bsize;
    					sum.os_bfree  += stat_buf.os_bfree *
    						stat_buf.os_bsize;
    					sum.os_bavail += stat_buf.os_bavail *
    						stat_buf.os_bsize;
    					ost_ffree += stat_buf.os_ffree;
    				}
    			}
    		}
    	}

    	close(fd);

    	/* If we don't have as many objects free on the OST as inodes
    	 * on the MDS, we reduce the total number of inodes to
    	 * compensate, so that the "inodes in use" number is correct.
    	 * Matches ll_statfs_internal() so the results are consistent. */
    	if (ost_ffree < sum.os_ffree) {
    		sum.os_files = (sum.os_files - sum.os_ffree) + ost_ffree;
    		sum.os_ffree = ost_ffree;
    	}

    	return rc;
    }

    int Lustre_Extractor::getname(std::string mntdir, mountpoint *mntpoint) {
        char buf[sizeof(obd_uuid)];
        int rc = llapi_getname(mntdir.c_str(), buf, sizeof(buf));

        if(rc != 0) {
            utils::log::logging::error() << "[Lustre_Extractor] error occured: "
                                         << std::strerror(errno);
        }
        mntpoint->name = buf;
    }

    bool const Lustre_Extractor::create_dummy_file(std::string const &path)
    {
        //setting arguments as documented in the lustre manual
        //to get system default values for file
        int stripe_size = 0;
        int stripe_offset = -1;
        int stripe_count = 0;
        int stripe_pattern = 0;
        int rc = 0;

        rc = llapi_file_create(path.c_str(), stripe_size,
                                             stripe_offset,
                                             stripe_count,
                                             stripe_pattern);

        //if error occured exept "file exist" error.
        if(rc) {
            if(errno != EEXIST) {
                utils::log::logging::error() << "[Lustre_Extractor] Could not create dummy file. Reason: "
                                             << std::strerror(errno);
                return false;
            }
            utils::log::logging::error() << "[Lustre_Extractor] dummy file allready exists. "
                                         << "Returned default values could be wrong. continue...";
        }

        return true;
    }

    bool Lustre_Extractor::file_getstripe(std::string const &path, data &result) {
        //TODO: why is lmm_objects empty?
        //TODO: why is lmm_stripe_offset 0 instead of 115???
        int v1, v3, join;
        v1 = sizeof(struct lov_user_md_v1) +
                LOV_MAX_STRIPE_COUNT * sizeof(struct lov_user_ost_data_v1);
        v3 = sizeof(struct lov_user_md_v3) +
                LOV_MAX_STRIPE_COUNT * sizeof(struct lov_user_ost_data_v1);
        lov_user_md_v1 *lum = NULL;
        lum = malloc(std::max(v1, v3));

        if(llapi_file_get_stripe(path.c_str(), lum) != 0) {
            utils::log::logging::error() << "[Lustre_Extractor] error while file_getstripe: "
                                         << std::strerror(errno);
            return false;
        }

        result.defaults.lmm_stripe_size = std::to_string(lum->lmm_stripe_size);
        result.defaults.lmm_stripe_count = std::to_string(lum->lmm_stripe_count);
        result.defaults.lmm_stripe_offset = std::to_string(lum->lmm_stripe_offset);
        result.defaults.lmm_pattern = std::to_string(lum->lmm_pattern);
        result.defaults.lmm_magic = std::to_string(lum->lmm_magic);
        result.defaults.lmm_layout_gen = std::to_string(lum->lmm_layout_gen);

        free(lum);
        return true;
    }

    void Lustre_Extractor::collect_mountpoint_data(data& result)
    {
        char fsname[PATH_MAX] = "";
        char path[PATH_MAX] = {'\0'};
        int index = 0;
        int rc = 0;
        char mntdir[PATH_MAX] = {'\0'};
	int ops = LL_STATFS_LMV | LL_STATFS_LOV;

        //TODO: automaticly generate a path? example by getting username
        std::string dummy_path = "/lustre/scratch2/s8946413/querytest/lustre_dummy_test_file";
        if(create_dummy_file(dummy_path)){
            file_getstripe(dummy_path, result);
        }

        //for every mountpoint do...
	while (!llapi_search_mounts(path, index++, mntdir, fsname)) {
		/* Check if we have a mount point */
		if (mntdir[0] == '\0')
			continue;

                mountpoint mntpoint;
                mntpoint.path = mntdir;
		rc = mntdf(mntdir, fsname, ops, NULL, &mntpoint);
                getname(std::string(mntdir), &mntpoint);
                result.mountpoints.push_back(mntpoint);

		if (rc || path[0] != '\0')
			break;
		fsname[0] = '\0'; /* avoid matching in next loop */
		mntdir[0] = '\0'; /* avoid matching in next loop */
	}
    }

}} /* closing namespace sysmap::linux */
