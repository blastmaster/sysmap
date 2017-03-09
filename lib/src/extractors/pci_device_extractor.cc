#include "value.hpp"
#include "scalar_value.hpp"
#include "array_value.hpp"
#include "map_value.hpp"
#include "utils.hpp"

#include "extractors/pci_device_extractor.hpp"

#include <sqlite_modern_cpp.h>

#include <cstdio>

using namespace sqlite;

namespace adafs { namespace extractor {

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

    PCI_Device_Extractor::PCI_Device_Extractor() : Extractor("Hwloc_PCI")
    {
        hwloc_topology_init(&topology);
        hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_WHOLE_IO);
        hwloc_topology_load(topology);
    }

    PCI_Device_Extractor::~PCI_Device_Extractor()
    {
        hwloc_topology_destroy(topology);
    }

    void PCI_Device_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();

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

    void PCI_Device_Extractor::store(Extractor_Set& findings, const std::string& dbname)
    {
        database db(dbname);
        adafs::utils::log::logging::debug() << "pci device extractor created database object: " << dbname;
        create_pci_device_table(db);
        adafs::utils::log::logging::debug() << "pci device extractor created pci_devices table";

        // insert pci devices
        auto pci_info = findings.get<Array_value>("PCIDevices");
        int inserted_pci_devices = insert_pci_devices(pci_info, db);
        adafs::utils::log::logging::debug() << "pci device extractor inserted pci devices: " << inserted_pci_devices;
    }

}} /* closing namesapce adafs::extractor */
