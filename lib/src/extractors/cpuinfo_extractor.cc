#include "array_value.hpp"
#include "map_value.hpp"
#include "scalar_value.hpp"
#include "value.hpp"

#include "extractors/cpuinfo_extractor.hpp"
#include "utils.hpp"
#include <exception>

namespace sysmap
{
namespace extractor
{

using Uint_value = Scalar_value<uint64_t>;
using Double_value = Scalar_value<double>;
using String_value = Scalar_value<std::string>;

Cpuinfo_Extractor::Cpuinfo_Extractor() : Extractor("Cpuinfo")
{
}

void
Cpuinfo_Extractor::add_cache(Extractor_Set& findings, const std::pair<uint64_t, std::string>& cache, const std::string & cache_level)
{
    if (!std::get<1>(cache).empty())
    {
        auto cache_value = make_value<Map_value>();
        cache_value->add("Size", make_value<Uint_value>(std::get<0>(cache)));
        cache_value->add("Unit", make_value<String_value>(std::get<1>(cache)));
        findings.add_info(cache_level, std::move(cache_value));
    }
}

void
Cpuinfo_Extractor::load(Extractor_Set& findings)
{
    try
    {
        auto data = collect();
        findings.add_info("CoreCount", make_value<Uint_value>(data.cpu_count));
        findings.add_info("ThreadPerCore", make_value<Uint_value>(data.thread_per_core));
        findings.add_info("CorePerSocket", make_value<Uint_value>(data.cores_per_socket));
        findings.add_info("SocketCount", make_value<Uint_value>(data.socket_count));

        findings.add_info("CpuFreq", make_value<Double_value>(data.cpu_freq));
        findings.add_info("CpuFreqMin", make_value<Double_value>(data.cpu_freq_min));
        findings.add_info("CpuFreqMax", make_value<Double_value>(data.cpu_freq_max));

        findings.add_info("ModelName", make_value<String_value>(data.model_name));

        auto flags = make_value<Array_value>();
        for (auto& f : data.cpu_flags)
        {
            flags->add(make_value<String_value>(f));
        }
        findings.add_info("CpuFlags", std::move(flags));

        add_cache(findings, data.l1_cache, "L1");
        add_cache(findings, data.l2_cache, "L2");
        add_cache(findings, data.l3_cache, "L3");
    }
    catch (std::exception& e)
    {
        sysmap::utils::log::logging::error() << "Error due collecting CPU information.\n";
        return;
    }
}

void
Cpuinfo_Extractor::store([[maybe_unused]] Extractor_Set& findings,
                         [[maybe_unused]] const std::string& dbname)
{
}

} // namespace extractor
} // namespace sysmap
