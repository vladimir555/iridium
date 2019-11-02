// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F
#define HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F


#include "iridium/smart_ptr.h"
#include "iridium/logging/event.h"

#include "sink.h"


namespace iridium {
namespace logging {
namespace implementation {


class CSinkConsole: public ISink, pattern::NonCopyable {
public:
    DEFINE_IMPLEMENTATION(CSinkConsole)
    explicit CSinkConsole(TEvent::TLevel const &level);

    void initialize() override;
    void finalize() override;
    void log(TEvent const &event) override;

private:
    TEvent::TLevel m_level;
};


} // implementation
} // logging
} // iridium


#endif // HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F
