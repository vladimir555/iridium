// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SINK_3A3CF594_E847_4869_B666_7DF6E46ADCCA
#define HEADER_SINK_3A3CF594_E847_4869_B666_7DF6E46ADCCA


#include "iridium/smart_ptr.h"
#include "iridium/pattern/initializable.h"

#include "iridium/enum.h"

#include "event.h"

#include <atomic>


namespace iridium {
namespace logging {


///
class ISink : public pattern::IInitializable {
public:
    DEFINE_SMART_PTR(ISink)
    ///
    DEFINE_ENUM(
        TType,
        FILE,
        CONSOLE
    )
    ///
    virtual ~ISink() = default;
    ///
    virtual void log(TEvent::TConstSharedPtr const &e) = 0;
    ///
    static std::string makeLine(TEvent::TConstSharedPtr const &e);
    ///
    static std::atomic<bool> is_gmt_time;
};


} // logger
} // iridium


#endif // HEADER_SINK_3A3CF594_E847_4869_B666_7DF6E46ADCCA
