#ifndef HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F
#define HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F


#include "utility/smart_ptr.h"
#include "utility/logging/sink.h"
#include "utility/logging/event.h"
#include "utility/threading/implementation/worker.h"

#include "sink.h"


namespace utility {
namespace logging {
namespace implementation {


class CSinkConsole:
    public std::enable_shared_from_this<CSinkConsole>,
    public CSink,
    public threading::IWorkerHandler<TEvent>
{
public:
    DEFINE_CREATE(CSinkConsole)
    explicit CSinkConsole(TEvent::TLevel const &level);
    virtual ~CSinkConsole() = default;

private:
    void handleItems(TItems const &events) override;
    void handleStart() override;
    void handleStop() override;
};


} // implementation
} // logging
} // utility


#endif // HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F
