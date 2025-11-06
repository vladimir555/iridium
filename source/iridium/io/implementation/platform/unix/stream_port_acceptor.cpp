#include "stream_port_acceptor.h"

#include "stream_port_peer.h"

#include <iridium/convertion/convert.h>
#include <iridium/assert.h>


#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <system_error>
#include <fcntl.h>
#include <signal.h>


using std::string;
using iridium::convertion::convert;
using iridium::assertExists;


namespace iridium {
namespace io {
namespace implementation {
namespace platform {
namespace unix_ {


CStreamPortAcceptor::CStreamPortAcceptor(URI const &uri)
:
    m_uri   (assertExists(URI::create(uri), "url is null")),
    m_fd    (0)
{}


void CStreamPortAcceptor::initialize() {
    try {
        if (m_fd)
            throw std::runtime_error("already initialized");

        std::string path;
        if (m_uri->getProtocol() == URI::TProtocol::IPC)
            path = "\0" + m_uri->getPath(); // abstract namespace
        else
            throw std::runtime_error(
                "protocol '" + convert<string>(m_uri->getProtocol()) + "' not implemented");

        struct sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;

        snprintf(addr.sun_path + 1, sizeof(addr.sun_path) - 1, "%s", path.c_str() + 1);

        socklen_t addr_len = static_cast<socklen_t>(
            offsetof(struct sockaddr_un, sun_path) +
            strnlen(path.c_str() + 1, sizeof(addr.sun_path) - 2) + 1
        );

        m_fd = assertOK(::socket(AF_UNIX, SOCK_STREAM, 0), "socket");
        static int const YES = 1;
        ::setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &YES, sizeof(YES));
        assertOK(::bind(m_fd, reinterpret_cast<struct sockaddr*>(&addr), addr_len), "bind");
        assertOK(::listen(m_fd, 5), "listen");

        setBlockingMode(false);

    } catch (std::exception const &e) {
        throw std::runtime_error(string("stream port acceptor initialization error: ") + e.what());
    }
}


void CStreamPortAcceptor::finalize() {
    if (m_fd) {
        close(m_fd);
        m_fd = 0;
    }
}


std::list<uintptr_t> CStreamPortAcceptor::getHandles() const {
    return {
        static_cast<uintptr_t>(m_fd)
    };
}


URI::TSharedPtr CStreamPortAcceptor::getURI() const {
    return m_uri;
}


std::list<IStreamPort::TSharedPtr> CStreamPortAcceptor::accept() {
    std::list<IStreamPort::TSharedPtr> peers;
    try {
        if (!m_fd)
            throw std::runtime_error("not initialized");

        struct sockaddr_un client_addr;
        socklen_t client_len = sizeof(client_addr);

        for (;;) {
            int peer_fd = ::accept(m_fd, reinterpret_cast<struct sockaddr*>(&client_addr), &client_len);
            if (peer_fd == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
                else
                    assertOK(peer_fd, "accept");
            }
            peers.push_back(CStreamPortPeer::create(*m_uri, peer_fd));
        }
    } catch (std::exception const &e) {
        throw std::runtime_error(string("stream port accepting error: ") + e.what());
    }

    return peers;
}


void CStreamPortAcceptor::setBlockingMode(bool const &is_blocking) {
    auto flags = assertOK(fcntl(m_fd, F_GETFL, 0), "get flag error, fd " + convert<std::string>(m_fd));
    if (is_blocking)
        flags &= !O_NONBLOCK;
    else
        flags |=  O_NONBLOCK;
    assertOK(fcntl(m_fd, F_SETFL, flags), "set flag error, fd " + convert<std::string>(m_fd));
}


} // unix
} // platform
} // implementation
} // io
} // iridium
