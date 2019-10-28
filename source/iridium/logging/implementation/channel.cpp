#include "channel.h"
#include "iridium/pattern/initializable.h"
#include "iridium/threading/implementation/mutex.h"
#include "iridium/threading/synchronized_scope.h"


using iridium::pattern::IInitializable;
using iridium::threading::implementation::CMutex;


namespace iridium {
namespace logging {
namespace implementation {


CChannel::CChannel()
:
    Synchronized(CMutex::create())
{}


void CChannel::initialize() {
    LOCK_SCOPE
    broadcast(&ISink::initialize);
}


void CChannel::finalize() {
    LOCK_SCOPE
    broadcast(&ISink::finalize);
}


void CChannel::attach(ISink::TSharedPtr sink) {
    LOCK_SCOPE
    Broadcaster<ISink>::attach(sink);
}


void CChannel::detach(ISink::TSharedPtr sink) {
    LOCK_SCOPE
    Broadcaster<ISink>::detach(sink);
}


void CChannel::log(TEvent &&e) {
    LOCK_SCOPE_FAST
    broadcast(&ISink::log, e);
}


} // implementation
} // logger
} // iridium
