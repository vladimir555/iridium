#include "ipc_stream.h"


#ifdef UNIX_PLATFORM


#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <dirent.h>
#include <csignal>

#include "iridium/convertion/convert.h"


using iridium::convertion::convert;
using iridium::io::URI;
using iridium::io::Buffer;
using std::string;


namespace iridium {
namespace system {
namespace implementation {
namespace platform {
namespace unix_ {


CIPCStream::CIPCStream(io::URI const &uri, bool const &is_server)
:
    CStreamPort (uri),
    m_is_server (is_server)
{}


void CIPCStream::initialize() {
    try {
        if (m_fd_reader || m_fd_writer)
            throw std::runtime_error("already initialized or not finalized");

        std::string path = "/" + m_uri->getPath();

        int fd = 0;

        if (m_is_server) {
            // rm old if exists
            unlink(path.c_str());
            // server create fifo
            fd = assertOK(mkfifo(path.c_str(), 0666), "mkfifo");
        }

        // server or client open fifo
        fd = assertOK(::open(path.c_str(), O_RDWR), "open fifo");

        m_fd_reader = m_fd_writer = fd;
        setBlockingMode(false);
    } catch (std::exception const &e) {
        finalize();
        throw std::runtime_error(std::string("ipc stream initialization error: ") + e.what());
    }
}


void CIPCStream::finalize() {
    if (m_fd_writer) {
        close(m_fd_writer);
        m_fd_writer = 0;
    }

    if (m_fd_reader) {
        close(m_fd_reader);
        m_fd_reader = 0;
    }
}


} // unix_
} // platform
} // implementation
} // system
} // iridium


#endif // UNIX_PLATFORM
