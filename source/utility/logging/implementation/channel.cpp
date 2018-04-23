#include "channel.h"
#include "utility/pattern/initializable.h"
#include "utility/threading/synchronized_scope.h"


using utility::pattern::IInitializable;


namespace utility {
namespace logging {
namespace implementation {


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
} // utility
