#include "stream_port_acceptor.h"


#ifdef WINDOWS_PLATFORM


#include "stream_port.h"
#include "iridium/assert.h"
#include <stdexcept>
#include <unordered_map>

using iridium::assertExists;
using std::string;


namespace iridium::io::implementation::platform {


CStreamPortAcceptor::CStreamPortAcceptor(URI const &uri) {
    throw std::runtime_error("not implemented");
}


void CStreamPortAcceptor::initialize() {
    throw std::runtime_error("not implemented");
}


void CStreamPortAcceptor::finalize() {
    throw std::runtime_error("not implemented");
}


std::list<uintptr_t> CStreamPortAcceptor::getHandles() const {
    throw std::runtime_error("not implemented");
}


URI::TSharedPtr CStreamPortAcceptor::getURI() const {
    throw std::runtime_error("not implemented");
}


std::list<IStreamPort::TSharedPtr> CStreamPortAcceptor::accept() {
    throw std::runtime_error("not implemented");
}


} // iridium::io::implementation::platform


#endif // WINDOWS_PLATFORM
