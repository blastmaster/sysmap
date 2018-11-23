#ifndef __SYSMAP_LUSTRE_EXTRACTOR_HPP__
#define __SYSMAP_LUSTRE_EXTRACTOR_HPP__

#include "../extractors/lustre_extractor.hpp"

#undef linux

namespace sysmap { namespace linux {

    struct Lustre_Extractor : extractor::Lustre_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<Lustre_Extractor>(); }

        protected:
            virtual data collect() override;

        private:

        //TODO: get a better place
        enum mntdf_flags {
        	MNTDF_INODES	= 0x0001,
        	MNTDF_COOKED	= 0x0002,
        	MNTDF_LAZY	= 0x0004,
        	MNTDF_VERBOSE	= 0x0008,
        	MNTDF_SHOW	= 0x0010,
        };

            static Registrar registrar;
            int mntdf(char *mntdir, char *fsname,
		      int ops, struct ll_statfs_buf *lsb,
                      mountpoint *mntpoint);
            int getname(std::string mntdir, mountpoint *mntpoint);

            /* TODO: create_dummy_file */
            //TODO: get striping foo to work
            int getstripe();
            int file_getstripe();

            void collect_mountpoint_data(data& result);
    };

}} /* closing namespace sysmap::linux */

#endif /* __SYSMAP_LUSTRE_EXTRACTOR_HPP__ */
