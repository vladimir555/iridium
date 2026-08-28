// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "stream_port_acceptor.h"


#ifdef WINDOWS_PLATFORM


#include "stream_port.h"
#include "iridium/assert.h"
#include <stdexcept>
#include <unordered_map>

using iridium::assertExists;
using std::string;


namespace iridium::io::implementation::platform {


CStreamPortAcceptor::CStreamPortAcceptor(URI const &) {
    throw std::runtime_error("not implemented");
}


void CStreamPortAcceptor::initialize() {
    throw std::runtime_error("not implemented");
}


void CStreamPortAcceptor::finalize() {
    throw std::runtime_error("not implemented");
}


IStream::TMapHandleTypeIdent CStreamPortAcceptor::getHandles() const {
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
