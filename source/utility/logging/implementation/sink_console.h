#ifndef HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F
#define HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F


#include "utility/smart_ptr.h"
#include "utility/logging/event.h"

#include "sink.h"


namespace utility {
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
} // utility


#endif // HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F
