#include "linux/cpuinfo_extractor.hpp"
#include "utils.hpp"

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

namespace adafs { namespace linux {

Registrar Cpuinfo_Extractor::registrar("cpuinfo", &Cpuinfo_Extractor::create);

Cpuinfo_Extractor::data Cpuinfo_Extractor::collect()
{
    data result;

    utils::file::for_each_line("/proc/cpuinfo", [&data](const std::string& line) {
            auto position = line.find(":");
            std::string name = line.substr(0, position);
            boost::trim(name);
            const std::string tail = line.substr(position);

            if (name == "flags") {
                std::vector<std::string> flags;
                boost::split(flags, tail, boost::is_any_of(","), boost::compress_token_on);
            }
    }

    return result;
}

}} // adafs::linux
