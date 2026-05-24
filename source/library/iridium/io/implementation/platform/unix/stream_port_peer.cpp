#include "stream_port_peer.h"


#ifdef UNIX_PLATFORM


#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <filesystem>
#include <stdexcept>

#include "iridium/convertion/convert.h"


using iridium::convertion::convert;
using iridium::io::URI;
using iridium::io::Buffer;
using std::string;


namespace iridium::io::implementation::platform::unix_ {


CStreamPortPeer::CStreamPortPeer(URI const &uri, int const &fd)
:
    CStreamPort(uri)
{
    m_fd_reader = fd;
    m_fd_writer = fd;
}


void CStreamPortPeer::initialize() {
    try {
        setBlockingMode(false);

    } catch (std::exception const &e) {
        closeFDs();
        throw std::runtime_error(
            std::string("client stream initialization error: ") + e.what()
        );
    }
}


void CStreamPortPeer::finalize() {
    closeFDs();
}


} // iridium::io::implementation::platform::unix_


#endif // UNIX_PLATFORM
