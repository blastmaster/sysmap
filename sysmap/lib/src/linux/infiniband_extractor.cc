#include <regex>

#include <boost/algorithm/string.hpp>

#include "linux/infiniband_extractor.hpp"
#include "utils.hpp"


namespace adafs { namespace linux {

    Infiniband_Extractor::data Infiniband_Extractor::collect()
    {
        data result;
        collect_ibnetdiscover(result);
        return result;
    }

    void Infiniband_Extractor::collect_ibnetdiscover(data& result)
    {
        utils::for_each_line("../test_data/ib_data/ibnetdiscover_plain_taurusi6593", [&](const std::string& line) {
                // define pattern for switch line
                std::regex switch_pattern(R"(Switch\s+(\d+)\s+\"([\w-]+)\"\s+#\s+\"(.*)\"(.*)$)");
                std::regex switch_connectivity_pattern(R"(^\[(\d+)\]\s+\"([\w-]+)\"\[(\d+)\](?:\(([^\)]+)\))?\s+#\s+\"([^\"]+)\"\s+(.*)$)");
                std::regex host_pattern(R"(^Ca\s+(\d+)\s+\"([\w-]+)\"\s+#\s+\"([^\"]+)\"$)");
                std::regex host_connectivity_pattern(R"(^\[(\d+)\]\(([^\)]+)\)\s+\"([^\"]+)\"\[(\d+)\]\s+#\s+lid\s+(\d+)\s+lmc\s+(\d+)\s+\"([^\"]+)\"\s+lid\s+(\d+)\s+(.*)$)");

                std::smatch matches;
                if (std::regex_search(line, matches, switch_pattern)) {
                    std::cout << "Found Switch:\n";
                    for (int i = 1; i < matches.size(); ++i) {
                        auto match_str = matches[i].str();
                        boost::trim(match_str);
                        std::cout << "\t" << match_str << "\n";
                    }
                    //return false; // break after first match, for debugging
                }
                else if (std::regex_search(line, matches, switch_connectivity_pattern)) {
                    std::cout << "Found Switch connectivity:\n";
                    for (int i = 1; i < matches.size(); ++i) {
                        auto match_str = matches[i].str();
                        boost::trim(match_str);
                        std::cout << "\t" << match_str << "\n";
                    }
                }
                else if (std::regex_search(line, matches, host_pattern)) {
                    std::cout << "Found Host:\n";
                    for (int i = 1; i < matches.size(); ++i) {
                        auto match_str = matches[i].str();
                        boost::trim(match_str);
                        std::cout << "\t" << match_str << "\n";
                    }
                }
                else if (std::regex_search(line, matches, host_connectivity_pattern)) {
                    std::cout << "Found Host connectivity:\n";
                    for (int i = 1; i < matches.size(); ++i) {
                        auto match_str = matches[i].str();
                        boost::trim(match_str);
                        std::cout << "\t" << match_str << "\n";
                    }
                }


                return true;
            });
    }

}} /* closing namespace adafs::linux */
