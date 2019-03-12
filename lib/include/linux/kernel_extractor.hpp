#ifndef __SYSMAP_LINUX_KERNEL_EXTRACTOR_HPP__
#define __SYSMAP_LINUX_KERNEL_EXTRACTOR_HPP__

#include "../extractors/kernel_extractor.hpp"

#undef linux

namespace sysmap { namespace linux {

    struct Kernel_Extractor : extractor::Kernel_Extractor
    {

        static std::unique_ptr<Extractor> create() { return std::make_unique<Kernel_Extractor>(); }

        /*
         *Constructor
         */
        Kernel_Extractor() {}

        /*
         * Virtual destructor
         */
        virtual ~Kernel_Extractor() {}

        protected:

            /*
             * Calls methods to collect information about the Kernel
             * Overrides virtual function from the base class
             */
            virtual data collect() override;

        private:
            static Registrar registrar;

            /*
             * Searches through paths (declared in base class) to locate kernel config file.
             * Reads kernel config using utils::file::for_each_line() and returns Kernel information in the structure "result".
             * @param result Reference to data object, gets filled with information.
             */
            void collect_kernel_config(data& result);

            std::string find_kernel_config(const data& result);

            /*
             * Reads /proc/modules and returns Module information in the structure "result".
             * @param result Reference to data object, gets filled with information.
             */
            void collect_kernel_modules(data& result);

            /*
             * Calls uname() to return information in the structure "result"
             * @param result Reference to data object, gets filled with information.
             */
            void collect_uname(data& result);

    };
} }

#endif /* ifndef __SYSMAP_LINUX_KERNEL_EXTRACTOR_HPP__ */
