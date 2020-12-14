// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CHANNEL_C7572D6C_E140_466A_8D8A_E3837B101B8A
#define HEADER_CHANNEL_C7572D6C_E140_466A_8D8A_E3837B101B8A


#include "iridium/smart_ptr.h"
#include "iridium/logging/channel.h"
#include "iridium/logging/event.h"
#include "iridium/logging/sink.h"
#include "iridium/pattern/broadcaster.h"
#include "iridium/pattern/non_copyable.h"
#include "iridium/threading/synchronized.h"


namespace iridium {
namespace logging {
namespace implementation {


class CChannel:
    public IChannel,
    public pattern::Broadcaster<ISink>,
    public pattern::NonCopyable,
    public threading::CSynchronized
{
public:
    DEFINE_IMPLEMENTATION(CChannel)
    CChannel();
    ///
    virtual void initialize() override;
    ///
    virtual void finalize() override;
    ///
    virtual void attach(ISink::TSharedPtr const &sink) override;
    ///
    virtual void detach(ISink::TSharedPtr const &sink) override;
    ///
    virtual void log(TEvent &&event) override;
};


} // implementation
} // logger
} // iridium


#endif // HEADER_CHANNEL_C7572D6C_E140_466A_8D8A_E3837B101B8A
