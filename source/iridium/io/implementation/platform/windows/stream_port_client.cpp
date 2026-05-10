#include "stream_port_client.h"


#ifdef WINDOWS_PLATFORM


#include "stream_port_acceptor.h"
#include "stream_port_peer.h"
#include "iridium/assert.h"
#include <stdexcept>


using iridium::assertExists;
using std::string;


namespace iridium::io::implementation::platform {


CStreamPortClient::CStreamPortClient(URI const &uri)
:
    CStreamPort(uri)
{
    throw std::runtime_error("not implemented");
}

void CStreamPortClient::initialize() {
    throw std::runtime_error("not implemented");
}


void CStreamPortClient::finalize() {
    throw std::runtime_error("not implemented");
}


} // iridium::io::implementation::platform


#endif // WINDOWS_PLATFORM
