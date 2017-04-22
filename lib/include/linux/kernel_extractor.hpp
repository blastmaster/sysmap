#ifndef __ADAFS_LINUX_KERNEL_EXTRACTOR_HPP__
#define __ADAFS_LINUX_KERNEL_EXTRACTOR_HPP__ 

#include "../extractors/kernel_extractor.hpp"

#undef linux

namespace adafs { namespace linux {

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
             * When succesful file gets copied to /tmp/kernel_config
             * @param result Result is used to get uname.release() (same as uname -r)
             * @return Returns true when kernel config file was found, else false
             */
            bool get_kernel_config(data& result);

            /*
             * Calls get_kernel_config(data& result), on success:
             * Reads /tmp/kernel_config and returns Kernel information in the structure "result".
             * @param result Reference to data object, gets filled with information.
             */
            void collect_kernel_config(data& result);

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

#endif /* ifndef __ADAFS_LINUX_KERNEL_EXTRACTOR_HPP__ */
