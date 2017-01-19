#ifndef __ADAFS_EXTRACTOR_HWLOC_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_HWLOC_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"

#include <vector>

extern "C" {
#include <hwloc.h>
}



namespace adafs { namespace extractor {

    struct Hwloc_Extractor : Extractor
    {
        Hwloc_Extractor();

        virtual ~Hwloc_Extractor();

        virtual void load(Extractor_Set& findings) override;
        virtual void store(Extractor_Set& findings, const std::string& dbname) override;

        protected:

        struct PCI_Device
        {
            std::string name;

            unsigned short domain; // 0000 to ffff

            unsigned char bus; // 0 to ff

            unsigned char dev; // 0 to 1f

            unsigned char func; // 0 to f

            unsigned short class_id; // %04x

            unsigned short vendor_id; // %04x

            unsigned short device_id; // %04x

            unsigned short subvendor_id;

            unsigned short subdevice_id;

            unsigned char revision; // %02x

            float linkspeed;

            std::string busid_str() const
            {
                char buf[8] = "";
                snprintf(buf, sizeof(buf), "%02x:%02x.%01x", bus, dev, func);
                return {buf};
            }

            std::string domain_str() const
            {
                char domain[6] = "";
                snprintf(domain, sizeof(domain), "%04x:", domain);
                return {domain};
            }


        };

        struct Machine_Info
        {
            // DMIProductName
            std::string product_name;
            // DMIProductVersion
            std::string product_version;
            // DMIBoardVendor
            std::string board_vendor;
            // DMIBoardName
            std::string board_name;
            // DMIBoardVersion
            std::string board_version;
            // DMIBoardAssetTag
            std::string board_asset_tag;
            // DMIChassisVendor
            std::string chassi_vendor;
            // DMIChassisType
            std::string chassi_type;
            // DMIChassisVersion
            std::string chassi_version;
            // DMIChassisAssetTag
            std::string chassi_asset_tag;
            // DMIBIOSVendor
            std::string bios_vendor;
            // DMIBIOSVersion
            std::string bios_version;
            // DMIBIOSDate
            std::string bios_date;
            // DMISysVendor
            std::string sys_vendor;
            // LinuxCgroup
            std::string linux_cgroup;
            // OSName
            std::string os_name;
            // OSRelease
            std::string os_release;
            // OSVersion
            std::string os_version;
            // HostName
            std::string hostname;
            // Architecture
            std::string architecture;
        };

        struct Memory_Info
        {
            // total memory in bytes
            uint64_t total_memory;
            // local memory in bytes
            uint64_t local_memory;

            struct Memory_Page_Type
            {
                Memory_Page_Type() : size{0}, count{0} {}

                Memory_Page_Type(uint64_t sz, uint64_t cnt) :
                    size{sz}, count{cnt} {}

                // size of pages
                uint64_t size;
                // number of pages of this size
                uint64_t count;
            };

            std::vector<Memory_Page_Type> page_types;
        };

        struct data
        {
            Machine_Info machine_info;

            Memory_Info memory_info;

            std::vector<PCI_Device> pci_devices;
        };

        virtual data collect() = 0;

        hwloc_topology_t topology;
    };

}} /* closing namespace adafs::extractor */

#endif /* __ADAFS_EXTRACTOR_HWLOC_EXTRACTOR_HPP__ */
