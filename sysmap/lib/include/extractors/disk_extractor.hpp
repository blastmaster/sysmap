#ifndef __ADAFS_EXTRACTOR_DISK_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_DISK_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"

#include <vector>

namespace adafs { namespace extractor {

    struct Disk_Extractor : Extractor {

        Disk_Extractor();

        virtual void load(Extractor_Set& findings) override;
        virtual void store(Extractor_Set& findings, const std::string& dbname) override;

        protected:

        struct Disk
        {
            std::string name;

            std::string vendor;

            std::string model;

            std::string product;

            std::string device_number;

            uint64_t size;
        };

        struct data
        {
            std::vector<Disk> disks;
        };

        virtual data collect() = 0;

    };

}} /* closing namespace adafs::extractor */

#endif /* __ADAFS_EXTRACTOR_DISK_EXTRACTOR_HPP__ */
