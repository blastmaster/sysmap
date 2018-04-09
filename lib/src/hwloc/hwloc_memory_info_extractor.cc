#include "hwloc/hwloc_memory_info_extractor.hpp"
#include "utils.hpp"


namespace sysmap { namespace hwloc {

    Registrar Hwloc_Memory_Info_Extractor::registrar("hwloc_memory", &Hwloc_Memory_Info_Extractor::create);

    Hwloc_Memory_Info_Extractor::data Hwloc_Memory_Info_Extractor::collect()
    {
        data result;
        collect_memory_info(result);

        return result;
    }
    
    void Hwloc_Memory_Info_Extractor::collect_memory_info(data& result)
    {
        // NOTE: just works on machine level, not sure if this is always
        // correct, if not, search the whole topology for memory information
        // and just start with the root obj.
        hwloc_obj_t root_obj = hwloc_get_root_obj(topology);
        if (!root_obj) {
            utils::log::logging::error() << "Error: no hwloc root obj!\n";
            return;
        }

        result.memory_info.total_memory = root_obj->memory.total_memory;
        result.memory_info.local_memory = root_obj->memory.local_memory;

        for (int i = 0; i < root_obj->memory.page_types_len; ++i) {
            result.memory_info.page_types.emplace_back(root_obj->memory.page_types[i].size,
                        root_obj->memory.page_types[i].count);
        }
    }
    
    Hwloc_Memory_Info_Extractor::Hwloc_Memory_Info_Extractor(){
        hwloc_topology_init(&topology);
        hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_WHOLE_IO);
        hwloc_topology_load(topology);
    }

    Hwloc_Memory_Info_Extractor::~Hwloc_Memory_Info_Extractor() {
        hwloc_topology_destroy(topology);
    }

}}
