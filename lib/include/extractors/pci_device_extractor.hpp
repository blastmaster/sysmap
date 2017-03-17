#ifndef __ADAFS_EXTRACTOR_PCI_DEVICE_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_PCI_DEVICE_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"
#include <vector>


namespace adafs {namespace extractor {

    struct PCI_Device_Extractor : Extractor
    {
        PCI_Device_Extractor();

        virtual ~PCI_Device_Extractor();

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

        struct data
        {
            std::vector<PCI_Device> pci_devices;
        };

        virtual data collect() = 0;
    };

}} /* closing namespace adafs::extractor */

#endif /* __ADAFS_EXTRACTOR_PCI_DEVICE_EXTRACTOR_HPP__ */
