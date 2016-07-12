#ifndef __ADAFS_PROC_PARTITIONS_HPP__
#define __ADAFS_PROC_PARTITIONS_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>

namespace adafs {

/**
 * Get Partitions of a System by parsing /proc/partitions.
 *
 * Partition is some basic abstraction type for a partition containing the
 * values from /proc/partitions and some utility functions for comparing with
 * other partition objects.
 */

struct Proc_Partition {

    std::string partition_name;
    unsigned int num_blocks;
    unsigned int major_dev_num;
    unsigned int minor_dev_num;

    Proc_Partition() {};

    Proc_Partition(const std::string& name,
              const unsigned int nb,
              const unsigned int min,
              const unsigned int maj) :
        partition_name(name),
        num_blocks(nb),
        major_dev_num(maj),
        minor_dev_num(min)
    {}

    Proc_Partition operator=(const Proc_Partition& p)
    {
        if (*this != p)
        {
            this->partition_name = p.partition_name;
            this->num_blocks = p.num_blocks;
            this->major_dev_num = p.major_dev_num;
            this->minor_dev_num = p.minor_dev_num;
        }

        return *this;
    }

    bool is_subpartition(const Proc_Partition& p) const
    {
        return this->major_dev_num == p.major_dev_num;
    }

    bool operator<(const Proc_Partition& p) const
    {
        return this->num_blocks < p.num_blocks;
    }

    bool operator>(const Proc_Partition& p) const
    {
        return this->num_blocks > p.num_blocks;
    }

    bool operator==(const Proc_Partition& p) const
    {
        return (this->major_dev_num == p.major_dev_num && 
                this->minor_dev_num == p.minor_dev_num &&
                this->num_blocks == p.num_blocks &&
                this->partition_name == p.partition_name);
    }

    bool operator!=(const Proc_Partition& p) const
    {
        return !(*this == p);
    }

    std::string device_path() const
    {
        return "/dev/" + partition_name;
    }

    //std::ostream& operator<<(std::ostream&, Proc_Partition&) const;
    std::ostream& operator<<(std::ostream&) const;
    static void load_partitions(const std::string&);
    static std::map<std::string, Proc_Partition> proc_partition_cache;
    static bool proc_partitions_cache_loaded;
    static std::vector<std::string> device_path_cache;

};

} /* closing namespace adafs */

#endif /* __ADAFS_PROC_PARTITIONS_HPP__ */
