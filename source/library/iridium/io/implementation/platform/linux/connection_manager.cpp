// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "connection_manager.h"


#ifdef LINUX_PLATFORM


#include <stdexcept>


namespace iridium::io::implementation::platform {


void CConnectionManager::initialize() {
    throw std::runtime_error("connection mnager not implemented for linux");
}


void CConnectionManager::finalize() {
    throw std::runtime_error("connection mnager not implemented for linux");
}


void CConnectionManager::manage(
    URI::TSharedPtr
        const &uri,
    IProtocol::TSharedPtr
        const &protocol)
{
    throw std::runtime_error("connection mnager not implemented for linux");
}


void CConnectionManager::manage(
    URI::TSharedPtr
        const &uri,
    IAcceptor::TSharedPtr
        const &acceptor)
{
    throw std::runtime_error("connection mnager not implemented for linux");
}


} // iridium::io::implementation::platform


#endif // LINUX_PLATFORM
