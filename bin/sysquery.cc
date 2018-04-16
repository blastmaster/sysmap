#include "utils.hpp"

#include <sqlite_modern_cpp.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

using namespace sysmap;
namespace po = boost::program_options;

int main(int argc, char** argv)
{

    po::options_description description("sysquery - query sysmap.db");

    description.add_options()
        ("help,h", "Display help message")
        ("filename,f", po::value<std::string>(), "Filename of the database file")
        ("output,o", po::value<std::string>(), "If specified, output will be written to given file")
        ("query-string,q", po::value<std::string>(), "Query String");
        /* ("ar,e", po::value<std::vector<std::string>>()->multitoken()->composing(), "Define which extractors are used."); */

    po::variables_map vm;
    std::string filename, query_string;

    try
    {
        po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << description;
            return 0;
        }

        if (vm.count("filename")) {
            filename = vm["filename"].as<std::string>();
            utils::log::logging::debug() << "[sysquery] set actual database filename to: [" << filename << "]\n";
            sqlite::database db(filename);
            utils::log::logging::debug() << "[sysquery] opened database file";
        }

        if (vm.count("query-string")){
            query_string = vm["query-string"].as<std::string>();
        }

    }
    catch (const po::error& ex) {
        utils::log::logging::error() << "[sysquery] Error, recieved Exception: " << ex.what() << "\n";
        return -1;
    }

    sqlite::database db(filename);
    /* std::stringstream sstr; */
    /* OStreamWrapper osw(os); */

    /* db << query_string */
    /*      >> [&](std::string Name, std::string foo, std::string faa) { */
    /*          query_results.push_back(Name); */
    /*          std::cout << Name << std::endl; */
    /*          std::cout << foo << std::endl; */
    /*      }; */

    return 0;
}
