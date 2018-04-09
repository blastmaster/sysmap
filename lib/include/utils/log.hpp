#ifndef __SYSMAP_UTILS_LOG_HPP__
#define __SYSMAP_UTILS_LOG_HPP__

#include <nitro/log/log.hpp>
#include <nitro/log/sink/stderr.hpp>

#include <nitro/log/attribute/hostname.hpp>
#include <nitro/log/attribute/message.hpp>
#include <nitro/log/attribute/timestamp.hpp>
#include <nitro/log/attribute/severity.hpp>

#include <nitro/log/filter/and_filter.hpp>
#include <nitro/log/filter/severity_filter.hpp>

namespace sysmap { namespace utils {

    namespace log
    {

        namespace detail
        {

            typedef nitro::log::record<nitro::log::message_attribute,
                        nitro::log::timestamp_attribute,
                        nitro::log::hostname_attribute,
                        nitro::log::severity_attribute> record;

            template<typename Record>
            class log_formater
            {
                public:
                    std::string format(Record& r)
                    {
                        std::stringstream s;

                        s << "SYSMAP-System-Map: [" << r.hostname() << "][" << r.timestamp().count() << "]["
                            << r.severity() << "]: " << r.message();

                        return s.str();
                    }
            };

            template<typename Record>
            using log_filter = nitro::log::filter::severity_filter<Record>;

        } /* closing namespace detail */

        typedef nitro::log::logger<detail::record, detail::log_formater,
                    nitro::log::sink::stderr, detail::log_filter> logging;

        inline void set_min_severity_level(nitro::log::severity_level sev)
        {
            detail::log_filter<detail::record>::set_severity(sev);
        }

    } /* closing namespace log */

    using log::logging;

}} /* closing namespace sysmap::utils */

#endif /* __SYSMAP_UTILS_LOG_HPP__ */
