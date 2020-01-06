#include "linux/swap_extractor.hpp"
#include "utils.hpp"

#include <boost/algorithm/string.hpp>

namespace sysmap { namespace linux {

    Registrar Swap_Extractor::registrar("swap", &linux::Swap_Extractor::create);

    Swap_Extractor::data Swap_Extractor::collect()
    {
        data result;
        collect_swap_info_data(result);
        return result;
    }

    void Swap_Extractor::collect_swap_info_data(data& result)
    {
        utils::file::for_each_line("/proc/swaps", [&](const std::string& line)
        {
            if (boost::starts_with(line, "Filename")) {
                return true;
            }
            std::vector<std::string> words;
            boost::split(words, line, boost::is_any_of("\t "), boost::token_compress_on);
            if (words.empty())
            {
                result.swap_is_active = false;
            }
            else
            {
                result.swap_is_active = true;
                result.info.filename = words.at(0);
                result.info.type = words.at(1);
                result.info.size = std::stoull(words.at(2));
                result.info.used = std::stoull(words.at(3));
            }

            return true;
        });
    }

}} /* closing namespace sysmap::linux */
