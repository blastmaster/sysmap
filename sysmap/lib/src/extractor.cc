#include "extractor.hpp"
#include "utils.hpp"

namespace adafs {

    std::unordered_map<std::string, Extractor::extractor_ptr* > &
    Extractor::registry()
    {
        static std::unordered_map<std::string, Extractor::extractor_ptr* > impl;
        return impl;
    }


} /* closing namespace adafs */
