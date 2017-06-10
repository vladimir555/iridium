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


//todo: async base sink with worker
class CSinkConsole : public CSink {
public:
    DEFINE_CREATE(CSinkConsole)
    ///
    explicit CSinkConsole(TEvent::TLevel const &level);
    ///
    virtual ~CSinkConsole() = default;

private:
    ///
    class CSinkWorker : public threading::implementation::CWorker<TEvent> {
    public:
        DEFINE_CREATE(CSinkWorker)
        CSinkWorker();
        virtual ~CSinkWorker() = default;
    private:
        virtual void handleItems(TItems const &events) override;
    };
};


} // implementation
} // logging
} // utility


#endif // HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F
