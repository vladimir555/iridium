/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
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
//:
//    Synchronized(CMutex::create())
{}


void CChannel::initialize() {
    LOCK_SCOPE
    broadcast(&ISink::initialize);
}


void CChannel::finalize() {
    LOCK_SCOPE
    broadcast(&ISink::finalize);
}


void CChannel::attach(ISink::TSharedPtr const &sink) {
    LOCK_SCOPE
    Broadcaster<ISink>::attach(sink);
}


void CChannel::detach(ISink::TSharedPtr const &sink) {
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
