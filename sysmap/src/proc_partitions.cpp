
#include "proc_partitions.hpp"


namespace adafs {


bool Proc_Partition::proc_partitions_cache_loaded = false;
std::map<std::string, Proc_Partition> Proc_Partition::proc_partition_cache;
std::vector<std::string> Proc_Partition::device_path_cache;


//std::ostream& Proc_Partition::operator<<(std::ostream& os, Proc_Partition& p) const
//{
    //os << "Major Device Number: " << p.major_dev_num << "\n";
    //os << "Minor Device Number: " << p.minor_dev_num << "\n";
    //os << "Number of Blocks: " << p.num_blocks << "\n";
    //os << "Partition name: " << p.partition_name << "\n";
    //return os;
//}

std::ostream& Proc_Partition::operator<<(std::ostream& os) const
{
    os << "Major Device Number: " << this->major_dev_num << "\n";
    os << "Minor Device Number: " << this->minor_dev_num << "\n";
    os << "Number of Blocks: " << this->num_blocks << "\n";
    os << "Partition name: " << this->partition_name << "\n";
    return os;
}

/**
 * load partitions from /proc/partitions file
 * stores the results in proc_partition_cache which is a map<device_path,Proc_Partition>
 */
void Proc_Partition::load_partitions(const std::string& fname)
{
    if (proc_partitions_cache_loaded) {
        return;
    }

    device_path_cache.clear();
    proc_partition_cache.clear();
    std::ifstream in(fname);

    if (!in) {
        std::cerr << "No file! " << fname << "\n";
        return;
    }

    for (std::string line; getline(in, line);) {
        std::regex pattern(R"(^\s+([0-9]+)\s+([0-9]+)\s+([0-9]+)\s+([^0-9]+.*)$)");
        std::smatch results;
        if (std::regex_search(line, results, pattern)) {
            Proc_Partition tmp(results[4].str(),
                    std::stoi(results[3].str()),
                    std::stoi(results[2].str()),
                    std::stoi(results[1].str()));
            auto devpath = tmp.device_path();
            device_path_cache.emplace_back(devpath);
            proc_partition_cache[devpath] = tmp;
        }
    }

    if (!device_path_cache.empty() && !proc_partition_cache.empty()) {
        proc_partitions_cache_loaded = true;
    }

}

} /* closing namespace adafs */
