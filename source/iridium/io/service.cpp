#include "service.h"

#include "implementation/listener.h"
#include "iridium/threading/implementation/mutex.h"
#include "iridium/threading/synchronized_scope.h"


namespace iridium {
namespace io {


Service::Service()
:
    threading::Synchronized(threading::implementation::CMutex::create()),
    m_listener      (implementation::CListener::create()),
    m_is_initialized(false)
{
    initialize();
}


Service::~Service() {
    finalize();
}


void Service::add(IStream::TSharedPtr const &stream) {
    LOCK_SCOPE
    m_listener->add(stream);
}


void Service::del(IStream::TSharedPtr const &stream) {
    LOCK_SCOPE
    m_listener->del(stream);
}


IListener::TEvents Service::wait() {
    LOCK_SCOPE
    return m_listener->wait(); // ----->
}


void Service::initialize() {
    LOCK_SCOPE
    if (m_is_initialized)
        return; // ----->
    m_listener->initialize();
    m_is_initialized = true;
}


void Service::finalize() {
    LOCK_SCOPE
    if (!m_is_initialized)
        return; // ----->
    m_listener->finalize();
    m_is_initialized = false;
}


} // io
} // iridium
