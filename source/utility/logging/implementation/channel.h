#ifndef HEADER_CHANNEL_C7572D6C_E140_466A_8D8A_E3837B101B8A
#define HEADER_CHANNEL_C7572D6C_E140_466A_8D8A_E3837B101B8A


#include "utility/smart_ptr.h"
#include "utility/logging/channel.h"
#include "utility/logging/event.h"
#include "utility/logging/sink.h"
#include "utility/pattern/broadcaster.h"
#include "utility/pattern/non_copyable.h"
#include "utility/threading/implementation/mutex.h"


namespace utility {
namespace logging {
namespace implementation {


class CChannel:
    public IChannel,
    public pattern::Broadcaster<ISink>,
    public pattern::NonCopyable,
    public threading::implementation::CMutex
{
public:
    DEFINE_CREATE(CChannel);
    ///
    CChannel() = default;
    ///
    virtual ~CChannel() = default;
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
    /////
    //virtual void log(std::atomic<std::unique_ptr<TEvent> > const &e);
};


} // implementation
} // logger
} // utility


#endif // HEADER_CHANNEL_C7572D6C_E140_466A_8D8A_E3837B101B8A
