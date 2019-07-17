#ifndef __SYSMAP_EXTRACTOR_CPUINFO_EXTRACTOR_HPP__
#define __SYSMAP_EXTRACTOR_CPUINFO_EXTRACTOR_HPP__

#include <utility>

#include "../extractor.hpp"
#include "../extractor_set.hpp"
#include <bits/stdint-uintn.h>

namespace sysmap
{
namespace extractor
{


struct Cpuinfo_Extractor : Extractor
{

    Cpuinfo_Extractor();

    virtual void load(Extractor_Set& findings) override;
    virtual void store(Extractor_Set& findings, const std::string& dbname) override;

protected:
    struct data
    {
        uint64_t cpu_count;
        uint64_t cores_per_socket;
        uint64_t socket_count;
        uint64_t thread_per_core;
        double cpu_freq_min;
        double cpu_freq_max;
        double cpu_freq;
        std::string model_name;
        std::vector<std::string> cpu_flags;
        std::pair<uint64_t, std::string> l1_cache;
        std::pair<uint64_t, std::string> l2_cache;
        std::pair<uint64_t, std::string> l3_cache;
    };

    virtual data collect() = 0;

private:
    void add_cache(Extractor_Set& findings, const std::pair<uint64_t, std::string>& cache, const std::string& cache_level);
};

} // namespace extractor
} // namespace sysmap

#endif /* __SYSMAP_EXTRACTOR_CPUINFO_EXTRACTOR_HPP__ */
