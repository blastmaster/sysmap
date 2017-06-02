#include "value.hpp"
#include "scalar_value.hpp"
#include "array_value.hpp"
#include "map_value.hpp"
#include "utils.hpp"

#include "extractors/pci_device_extractor.hpp"

#include <cstdio>

namespace adafs { namespace extractor {

    PCI_Device_Extractor::PCI_Device_Extractor() : Extractor("PCI_Extractor")
    {
    }

    PCI_Device_Extractor::~PCI_Device_Extractor()
    {
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
    }

}} /* closing namesapce adafs::extractor */
