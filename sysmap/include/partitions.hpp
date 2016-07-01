#ifndef __ADAFS_PARTITIONS_HPP__
#define __ADAFS_PARTITIONS_HPP__

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

namespace adafs {

/**
 * Get Partitions of a System by parsing /proc/partitions.
 *
 * Partition is some basic abstraction type for a partition containing the
 * values from /proc/partitions and some utility functions for comparing with
 * other partition objects.
 */


struct Partition {
    const std::string partition_name;
    const unsigned int num_blocks;
    const unsigned int major_dev_num;
    const unsigned int minor_dev_num;

    Partition(const std::string& name,
              const unsigned int nb,
              const unsigned int min,
              const unsigned int maj) :
        partition_name(name),
        num_blocks(nb),
        major_dev_num(maj),
        minor_dev_num(min)
    {}

    bool is_subpartition(const Partition& p) const
    {
        return this->major_dev_num == p.major_dev_num;
    }

    bool operator<(const Partition& p) const
    {
        return this->num_blocks < p.num_blocks;
    }

    bool operator>(const Partition& p) const
    {
        return this->num_blocks > p.num_blocks;
    }

    bool operator==(const Partition& p) const
    {
        return (this->major_dev_num == p.major_dev_num && 
                this->minor_dev_num == p.minor_dev_num &&
                this->num_blocks == p.num_blocks &&
                this->partition_name == p.partition_name);
    }

    bool operator!=(const Partition& p) const
    {
        return !(*this == p);
    }
};


std::ostream& operator<<(std::ostream& os, const Partition& p)
{
    os << "Major Device Number: " << p.major_dev_num << "\n";
    os << "Minor Device Number: " << p.minor_dev_num << "\n";
    os << "Number of Blocks: " << p.num_blocks << "\n";
    os << "Partition name: " << p.partition_name << "\n";
    return os;
}

/**
 * load partitions from /proc/partitions file
 */
static
std::vector<Partition> load_partitions(const std::string& fname)
{
    std::ifstream in(fname);
    std::vector<Partition> partitions;

    if (!in) {
        std::cerr << "No file!\n";
        return partitions;
    }

    for (std::string line; getline(in, line);) {
        std::regex pattern(R"(^\s+([0-9]+)\s+([0-9]+)\s+([0-9]+)\s+([^0-9]+.*)$)");
        std::smatch results;
        if (std::regex_search(line, results, pattern)) {
            //std::cout << "Match, " << results.size() << " times!\n";
            //for (size_t i = 0; i < results.size(); ++i) {
                //std::cout << i << ": " << results[i] << "\n";
            //}
            //FIXME; results is of type std::smatch need some better
            //initialization than this smatch -> string -> const char* -> int
            //conversion cascade.
            Partition tmp(results[4],
                    atoi(results[3].str().c_str()),
                    atoi(results[2].str().c_str()),
                    atoi(results[1].str().c_str()));
            partitions.push_back(tmp);
        }
    }

    in.close();
    return partitions;
}

} /* closing namespace adafs */

#endif /* __ADAFS_PARTITIONS_HPP__ */
