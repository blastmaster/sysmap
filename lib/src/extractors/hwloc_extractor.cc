#include "value.hpp"
#include "scalar_value.hpp"
#include "array_value.hpp"
#include "map_value.hpp"
#include "utils.hpp"

#include "extractors/hwloc_extractor.hpp"

#include <sqlite_modern_cpp.h>

#include <cstdio>

using namespace sqlite;

namespace adafs { namespace extractor {

    static void create_machine_info_table(database& db)
    {
        db << "CREATE TABLE IF NOT EXISTS machine_info (\
                product_name string,\
                product_version string,\
                board_vendor string,\
                board_name string,\
                board_version string,\
                board_asset_tag string,\
                chassi_vendor string,\
                chassi_type string,\
                chassi_version string,\
                chassi_asset_tag string,\
                bios_vendor string,\
                bios_version string,\
                bios_date string,\
                sys_vendor string,\
                linux_cgroup string,\
                os_name string,\
                os_release string,\
                os_version string,\
                hostname string not null primary key,\
                architecture string\
            );";
    }

    static void create_memory_info_table(database& db)
    {
        db << "CREATE TABLE IF NOT EXISTS memory_info (\
                total_memory integer,\
                local_memory integer\
            );";
    }

    static void create_pci_device_table(database db)
    {
        db << "CREATE TABLE IF NOT EXISTS pci_devices (\
                name string,\
                domain string,\
                busid string,\
                class_id integer,\
                vendor_id integer,\
                device_id integer,\
                subvendor_id integer,\
                subdevice_id integer,\
                revision integer,\
                linkspeed float\
            );";
    }

    static void insert_pci_device(const std::string& name,
            const std::string& domain,
            const std::string& busid,
            const unsigned int class_id,
            const unsigned int vendor_id,
            const unsigned int device_id,
            const unsigned int subvendor_id,
            const unsigned int subdevice_id,
            const unsigned int revision,
            const float linkspeed, database& db)
    {
        db << "INSERT INTO pci_devices VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
            << name
            << domain
            << busid
            << (int) class_id
            << (int) vendor_id
            << (int) device_id
            << (int) subvendor_id
            << (int) subdevice_id
            << (int) revision
            << linkspeed;
    }

    static int insert_pci_devices(const Array_value* pci_devs, database& db)
    {
        int inserted = 0;
        for (const auto& pci_dev : *pci_devs) {
            auto dev_map = pci_dev->as<Map_value>();
            std::string name = dev_map->get<String_value>("name")->value();
            std::string domain = dev_map->get<String_value>("domain")->value();
            std::string busid = dev_map->get<String_value>("busid")->value();
            unsigned int class_id = dev_map->get<Uint_value>("class_id")->value();
            unsigned int vendor_id = dev_map->get<Uint_value>("vendor_id")->value();
            unsigned int device_id = dev_map->get<Uint_value>("device_id")->value();
            unsigned int subvendor_id = dev_map->get<Uint_value>("subvendor_id")->value();
            unsigned int subdevice_id = dev_map->get<Uint_value>("subdevice_id")->value();
            unsigned int revision = dev_map->get<Uint_value>("revision")->value();
            double linkspeed = dev_map->get<Double_value>("linkspeed")->value();

            insert_pci_device(name, domain, busid, class_id, vendor_id, device_id,
                    subvendor_id, subdevice_id, revision, linkspeed, db);
            ++inserted;
        }

        return inserted;
    }

    static void insert_memory_info(const uint64_t tmem, const uint64_t lmem, database& db)
    {
        db << "INSERT INTO memory_info VALUES (?, ?);"
            << (int) tmem
            << (int) lmem;
    }

    static void insert_machine_info(const std::string& name,
            const std::string& product_version,
            const std::string& board_vendor,
            const std::string& board_name,
            const std::string& board_version,
            const std::string& board_asset_tag,
            const std::string& chassi_vendor,
            const std::string& chassi_type,
            const std::string& chassi_version,
            const std::string& chassi_asset_tag,
            const std::string& bios_vendor,
            const std::string& bios_version,
            const std::string& bios_date,
            const std::string& sys_vendor,
            const std::string& linux_cgroup,
            const std::string& os_name,
            const std::string& os_release,
            const std::string& os_version,
            const std::string& hostname,
            const std::string& architecture, database& db)
    {
        db << "INSERT INTO machine_info VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
            << name
            << product_version
            << board_vendor
            << board_name
            << board_version
            << board_asset_tag
            << chassi_vendor
            << chassi_type
            << chassi_version
            << chassi_asset_tag
            << bios_vendor
            << bios_version
            << bios_date
            << sys_vendor
            << linux_cgroup
            << os_name
            << os_release
            << os_version
            << hostname
            << architecture;
    }


    Hwloc_Extractor::Hwloc_Extractor() : Extractor("Hwloc")
    {
        hwloc_topology_init(&topology);
        hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_WHOLE_IO);
        hwloc_topology_load(topology);
    }

    Hwloc_Extractor::~Hwloc_Extractor()
    {
        hwloc_topology_destroy(topology);
    }

    void Hwloc_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();

        auto machine = make_value<Map_value>();

        machine->add("ProductName", make_value<String_value>(std::move(data.machine_info.product_name)));
        machine->add("ProductVersion", make_value<String_value>(std::move(data.machine_info.product_version)));
        machine->add("BoardVendor", make_value<String_value>(std::move(data.machine_info.board_vendor)));
        machine->add("BoardName", make_value<String_value>(std::move(data.machine_info.board_name)));
        machine->add("BoardVersion", make_value<String_value>(std::move(data.machine_info.board_version)));
        machine->add("BoardAssetTag", make_value<String_value>(std::move(data.machine_info.board_asset_tag)));
        machine->add("ChassisVendor", make_value<String_value>(std::move(data.machine_info.chassi_vendor)));
        machine->add("ChassisType", make_value<String_value>(std::move(data.machine_info.chassi_type)));
        machine->add("ChassisVersion", make_value<String_value>(std::move(data.machine_info.chassi_version)));
        machine->add("ChassisAssetTag", make_value<String_value>(std::move(data.machine_info.chassi_asset_tag)));
        machine->add("BiosVendor", make_value<String_value>(std::move(data.machine_info.bios_vendor)));
        machine->add("BiosVersion", make_value<String_value>(std::move(data.machine_info.bios_version)));
        machine->add("BiosDate", make_value<String_value>(std::move(data.machine_info.bios_date)));
        machine->add("SysVendor", make_value<String_value>(std::move(data.machine_info.sys_vendor)));
        machine->add("LinuxCgroup", make_value<String_value>(std::move(data.machine_info.linux_cgroup)));
        machine->add("OsName", make_value<String_value>(std::move(data.machine_info.os_name)));
        machine->add("OsRelease", make_value<String_value>(std::move(data.machine_info.os_release)));
        machine->add("OsVersion", make_value<String_value>(std::move(data.machine_info.os_version)));
        machine->add("Hostname", make_value<String_value>(std::move(data.machine_info.hostname)));
        machine->add("Architecture", make_value<String_value>(std::move(data.machine_info.architecture)));

        findings.add_info("machineinfo", std::move(machine));

        auto memory = make_value<Map_value>();

        memory->add("TotalMemory", make_value<Uint_value>(data.memory_info.total_memory));
        memory->add("LocalMemory", make_value<Uint_value>(data.memory_info.local_memory));

        auto pages = make_value<Array_value>();
        for (const auto& page : data.memory_info.page_types) {
            auto value = make_value<Map_value>();

            value->add("PageSize", make_value<Uint_value>(page.size));
            value->add("Count", make_value<Uint_value>(page.count));

            pages->add(std::move(value));
        }
        memory->add("PageTypes", std::move(pages));
        findings.add_info("memoryinfo", std::move(memory));

        auto pci_devices = make_value<Array_value>();
        for (const auto& pdev : data.pci_devices) {
            auto value = make_value<Map_value>();
            value->add("name", make_value<String_value>(pdev.name));
            // Handle busid and domain as string.
            value->add("domain", make_value<String_value>(pdev.domain_str()));
            //value->add("domain", make_value<Ushort_value>(pdev.domain));
            value->add("busid", make_value<String_value>(pdev.busid_str()));
            //value->add("bus", make_value<Uchar_value>(pdev.bus));
            //value->add("dev", make_value<Uchar_value>(pdev.dev));
            //value->add("func", make_value<Uchar_value>(pdev.func));

            //FIXME: handle the others as unsigend int values
            value->add("class_id", make_value<Uint_value>(pdev.class_id));
            value->add("vendor_id", make_value<Uint_value>(pdev.vendor_id));
            value->add("device_id", make_value<Uint_value>(pdev.device_id));
            value->add("subvendor_id", make_value<Uint_value>(pdev.subvendor_id));
            value->add("subdevice_id", make_value<Uint_value>(pdev.subdevice_id));
            value->add("revision", make_value<Uint_value>(pdev.revision));
            value->add("linkspeed", make_value<Double_value>(pdev.linkspeed));

            pci_devices->add(std::move(value));
        }
        findings.add_info("pcidevices", std::move(pci_devices));
    }

    void Hwloc_Extractor::store(Extractor_Set& findings, const std::string& dbname)
    {
        database db(dbname);
        adafs::utils::log::logging::debug() << "hwloc extractor created database object: " << dbname;
        create_machine_info_table(db);
        adafs::utils::log::logging::debug() << "hwloc extractor created machine_info table";
        create_memory_info_table(db);
        adafs::utils::log::logging::debug() << "hwloc extractor created memory_info table";
        create_pci_device_table(db);
        adafs::utils::log::logging::debug() << "hwloc extractor created pci_devices table";


        // FIXME some of the ugliest code i've ever written
        // insert machine_info
        auto m_info = findings.get<Map_value>("MachineInfo");
        std::string name = m_info->get<String_value>("ProductName")->value();
        std::string product_version = m_info->get<String_value>("ProductVersion")->value();
        std::string board_vendor = m_info->get<String_value>("BoardVendor")->value();
        std::string board_name = m_info->get<String_value>("BoardName")->value();
        std::string board_version = m_info->get<String_value>("BoardVersion")->value();
        std::string board_asset_tag = m_info->get<String_value>("BoardAssetTag")->value();
        std::string chassi_vendor = m_info->get<String_value>("ChassisVendor")->value();
        std::string chassi_type = m_info->get<String_value>("ChassisType")->value();
        std::string chassi_version = m_info->get<String_value>("ChassisVersion")->value();
        std::string chassi_asset_tag = m_info->get<String_value>("ChassisAssetTag")->value();
        std::string bios_vendor = m_info->get<String_value>("BiosVendor")->value();
        std::string bios_version = m_info->get<String_value>("BiosVersion")->value();
        std::string bios_date = m_info->get<String_value>("BiosDate")->value();
        std::string sys_vendor = m_info->get<String_value>("SysVendor")->value();
        std::string linux_cgroup = m_info->get<String_value>("LinuxCgroup")->value();
        std::string os_name = m_info->get<String_value>("OsName")->value();
        std::string os_release = m_info->get<String_value>("OsRelease")->value();
        std::string os_version = m_info->get<String_value>("OsVersion")->value();
        std::string hostname = m_info->get<String_value>("Hostname")->value();
        std::string architecture = m_info->get<String_value>("Architecture")->value();

        insert_machine_info(name, product_version, board_vendor, board_name, board_version, board_asset_tag,
                chassi_vendor, chassi_type, chassi_version, chassi_asset_tag,
                bios_vendor, bios_version, bios_date, sys_vendor, linux_cgroup,
                os_name, os_release, os_version, hostname, architecture, db);
        adafs::utils::log::logging::debug() << "hwloc extractor insterted machine info";

        // insert memory info
        auto mem_info = findings.get<Map_value>("MemoryInfo");
        uint64_t totalmem = mem_info->get<Uint_value>("TotalMemory")->value();
        uint64_t localmem = mem_info->get<Uint_value>("LocalMemory")->value();

        insert_memory_info(totalmem, localmem, db);
        adafs::utils::log::logging::debug() << "hwloc extractor inserted memory info";

        // insert pci devices
        auto pci_info = findings.get<Array_value>("PCIDevices");
        int inserted_pci_devices = insert_pci_devices(pci_info, db);
        adafs::utils::log::logging::debug() << "hwloc extractor inserted pci devices: " << inserted_pci_devices;
    }

}} /* closing namesapce adafs::extractor */
