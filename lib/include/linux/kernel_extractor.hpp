#ifndef __ADAFS_LINUX_KERNEL_EXTRACTOR_HPP__
#define __ADAFS_LINUX_KERNEL_EXTRACTOR_HPP__ 

#include "../extractors/kernel_extractor.hpp"

#undef linux

namespace adafs { namespace linux {

    struct Kernel_Extractor : extractor::Kernel_Extractor
    {

        static std::unique_ptr<Extractor> create() { return std::make_unique<Kernel_Extractor>(); }

        Kernel_Extractor() {}

        virtual ~Kernel_Extractor() {}

        protected:
            virtual data collect() override;

        private:
            static Registrar registrar;
            void collect_kernel_config(data& result);
            void collect_kernel_modules(data& result);
            void collect_uname(data& result);

    };
} }

#endif /* ifndef __ADAFS_LINUX_KERNEL_EXTRACTOR_HPP__ */
