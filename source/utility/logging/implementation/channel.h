#ifndef HEADER_CHANNEL_C7572D6C_E140_466A_8D8A_E3837B101B8A
#define HEADER_CHANNEL_C7572D6C_E140_466A_8D8A_E3837B101B8A


#include "utility/smart_ptr.h"
#include "utility/logging/channel.h"
#include "utility/logging/event.h"
#include "utility/logging/sink.h"
#include "utility/pattern/broadcaster.h"
#include "utility/pattern/non_copyable.h"
#include "utility/threading/synchronized.h"


namespace utility {
namespace logging {
namespace implementation {


class CChannel:
    public IChannel,
    public pattern::Broadcaster<ISink>,
    public pattern::NonCopyable,
    public threading::Synchronized
{
public:
    DEFINE_IMPLEMENTATION(CChannel)
    CChannel();
    ///
    virtual void initialize() override;
    ///
    virtual void finalize() override;
    ///
    virtual void attach(ISink::TSharedPtr sink) override;
    ///
    virtual void detach(ISink::TSharedPtr sink) override;
    ///
    virtual void log(TEvent &&event) override;
};


} // implementation
} // logger
} // utility


#endif // HEADER_CHANNEL_C7572D6C_E140_466A_8D8A_E3837B101B8A
