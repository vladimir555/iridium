#ifndef HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F
#define HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F


#include "utility/smart_ptr.h"
#include "utility/logging/event.h"

#include "sink.h"


namespace utility {
namespace logging {
namespace implementation {


class CSinkConsole: public CSink {
public:
    DEFINE_IMPLEMENTATION(CSinkConsole)
    explicit CSinkConsole(TEvent::TLevel const &level);

private:
    class CWorkerHandler: public threading::IWorkerHandler<TEvent> {
    public:
        DEFINE_IMPLEMENTATION(CWorkerHandler)
        TItems handle(TItems const &events) override;
        void initialize() override;
        void finalize() override;
    };
};


} // implementation
} // logging
} // utility


#endif // HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F
