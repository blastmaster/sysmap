#ifndef __ADAFS_EXTRACTOR_MEMORY_INFO_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_MEMORY_INFO_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"
#include <vector>


namespace adafs {namespace extractor {

    struct Memory_Info_Extractor : Extractor
    {
        Memory_Info_Extractor();

        virtual ~Memory_Info_Extractor();

        virtual void load(Extractor_Set& findings) override;
        virtual void store(Extractor_Set& findings, const std::string& dbname) override;

        protected:
        
        struct Memory_Info
        {
            // total memory in bytes
            uint64_t total_memory;
            // local memory in bytes
            uint64_t local_memory;

            struct Memory_Page_Type
            {
                Memory_Page_Type() : size{0}, count{0} {}

                Memory_Page_Type(uint64_t sz, uint64_t cnt) :
                    size{sz}, count{cnt} {}

                // size of pages
                uint64_t size;
                // number of pages of this size
                uint64_t count;
            };

            std::vector<Memory_Page_Type> page_types;
        };

        struct data
        {
            Memory_Info memory_info;
        };

        virtual data collect() = 0;
    };

}} /* closing namespace adafs::extractor */

#endif /* __ADAFS_EXTRACTOR_MEMORY_INFO_EXTRACTOR_HPP__ */
