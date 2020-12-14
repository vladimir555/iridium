#include "service.h"

#include "implementation/listener.h"


namespace iridium {
namespace io {


Service::Service()
:
    m_listener(implementation::CListener::create())
{
    m_listener->initialize();
}


Service::~Service() {
    m_listener->finalize();
}


void Service::add(IStream::TSharedPtr const &stream) {
    m_listener->add(stream);
}


void Service::del(IStream::TSharedPtr const &stream) {
    m_listener->del(stream);
}


IListener::TEvents Service::wait() {
    return m_listener->wait(); // ----->
}


void Service::initialize() {}


void Service::finalize() {}


threading::IMutex::TSharedPtr Service::getMutex() const {
    return nullptr; // ----->
}


} // io
} // iridium
