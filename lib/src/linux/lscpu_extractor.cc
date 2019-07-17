#include "linux/lscpu_extractor.hpp"
#include "utils.hpp"
#include "utils/execution.hpp"

#include <algorithm>
#include <bits/stdint-uintn.h>
#include <boost/algorithm/string/predicate.hpp>
#include <stdexcept>
#include <string>
#include <tuple>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <utility>

namespace fs = boost::filesystem;
namespace alg = boost::algorithm;

namespace sysmap { namespace linux {

Registrar Lscpu_Extractor::registrar("lscpu", &Lscpu_Extractor::create);

template<typename T>
void match_convert(std::string &               input,
                   const std::string &         pattern,
                   T &                         output,
                   std::function<T(std::string &)> converter )
{
    auto get_value = [](std::string & input) -> std::string
    {
        std::vector<std::string> strs;
        alg::split(strs, input, boost::is_any_of(":"));
        return alg::trim_copy(strs[1]);
    };

    if(alg::starts_with(input, pattern))
    {
        auto value = get_value(input);
        output = converter(value);
    }
}

Lscpu_Extractor::data Lscpu_Extractor::collect()
{
    std::vector<std::string> args;
    data result;

    using value_unit_pair = std::pair<uint64_t, std::string>;

    auto ends_successful = utils::exec::for_each_line("lscpu", args,
        [&result](std::string& input){
            auto convert2u64 = [](std::string & s) -> uint64_t {
                return std::stoull(s);
            };
            auto convert2double = [](std::string & s) -> double {
                return std::stod(s);
            };
            auto convert2pair = [](std::string & s) -> value_unit_pair {
                std::vector<std::string> strs;
                alg::split(strs, s, boost::is_any_of(" "));
                std::for_each(strs.begin(), strs.end(), [](std::string & in){
                    alg::trim(in);
                });
                if(strs.size() != 2)
                {
                    throw std::runtime_error("Failure occurred while converting string to pair.");
                }
                return std::make_pair(std::stoull(strs[0]), strs[1]);
            };
            if(alg::starts_with(input, "Flags:"))
            {
                std::vector<std::string> strs;
                alg::split(strs, input, boost::is_any_of(":"));

                alg::trim(strs[1]);
                alg::split(result.cpu_flags, strs[1], boost::is_any_of(" "));
                return true;
            }

            match_convert<uint64_t>(input, "CPU(s):", result.cpu_count, convert2u64);
            match_convert<uint64_t>(input, "Thread(s) per core:", result.thread_per_core, convert2u64);
            match_convert<uint64_t>(input, "Socket(s):", result.socket_count, convert2u64);
            match_convert<uint64_t>(input, "Core(s) per socket:", result.cores_per_socket, convert2u64);

            match_convert<double>(input, "CPU MHz:", result.cpu_freq, convert2double);
            match_convert<double>(input, "CPU min MHz:", result.cpu_freq_min, convert2double);
            match_convert<double>(input, "CPU max MHz:", result.cpu_freq_max, convert2double);

            match_convert<value_unit_pair>(input, "L1d cache:", result.l1_cache, convert2pair);
            match_convert<value_unit_pair>(input, "L2 cache:", result.l2_cache, convert2pair);
            match_convert<value_unit_pair>(input, "L3 cache:", result.l3_cache, convert2pair);

            match_convert<std::string>(input, "Model name:", result.model_name,
                                        [](std::string & s){return s;});

            return true;
    });
    if(! ends_successful)
    {
        throw utils::exec::execution_error("Failed to execute lscpu.");
    }
    return result;
}

}} // sysmap::linux
