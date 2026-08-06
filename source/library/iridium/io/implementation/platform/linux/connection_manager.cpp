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
