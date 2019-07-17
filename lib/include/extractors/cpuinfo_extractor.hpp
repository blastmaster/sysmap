#ifndef __SYSMAP_EXTRACTOR_CPUINFO_EXTRACTOR_HPP__
#define __SYSMAP_EXTRACTOR_CPUINFO_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"

namespace sysmap { namespace extractor {

//namespace fs = boost::filesystem;

struct Cpuinfo_Extractor : Extractor {

    Cpuinfo_Extractor();

    virtual void load(Extractor_Set& findings) override;
    virtual void store(Extractor_Set& findings, const std::string& dbname) override;

protected:

    struct Cpuinfo
    {
        std::vector<std::string> flags;
    };

    struct Arbitrary_Cpuinfo
    {
        std::map<std::string, std::string> info_map;
    };

    struct data
    {
        std::vector<Cpuinfo> cpuinfo;
        std::vector<Arbitrary_Cpuinfo> arbritray_cpuinfo;
    };

    virtual data collect() = 0;

};

}} // sysmap::extractor

#endif /* __SYSMAP_EXTRACTOR_CPUINFO_EXTRACTOR_HPP__ */
