#ifndef __SYSMAP_EXTRACTOR_MACHINE_INFO_EXTRACTOR_HPP__
#define __SYSMAP_EXTRACTOR_MACHINE_INFO_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"
#include <vector>
#include <map>


namespace sysmap { namespace extractor {

    /**
     * @class Machine_Info_Extractor
     * Machine_info_Extractor type abstract base class
     */

    struct Machine_Info_Extractor : Extractor
    {
        /*
         * Constructor
         */
        Machine_Info_Extractor() : Extractor("Machine_Info") {}

        /*
         * Virtual desructor
         */
        virtual ~Machine_Info_Extractor() {}

        virtual void load(Extractor_Set& findings) override;
        virtual void store(Extractor_Set& findings, const std::string& dbname) override;

        protected:

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

        struct data
        {
            Machine_Info machine_info;
        };
        
        virtual data collect() = 0;
    };

}} /* closing namespace sysmap::extractor */

#endif /* __SYSMAP_EXTRACTOR_MACHINE_INFO_EXTRACTOR_HPP__ */
