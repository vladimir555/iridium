#include "stream_port_ipc_client.h"


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


namespace iridium {
namespace io {
namespace implementation {
namespace platform {
namespace unix_ {


CStreamPortIPCClient::CStreamPortIPCClient(io::URI const &uri)
:
    CStreamPort (uri)
{}


void CStreamPortIPCClient::initialize() {
    try {
        if (m_fd_reader != -1 || m_fd_writer != -1)
            throw std::runtime_error("already initialized or not finalized");

        std::string path = "\0" + m_uri->getPath(); // abstract namespace

        struct sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;

        // Копируем имя сокета в sun_path, начиная со второго байта (после '\0')
        strncpy(addr.sun_path + 1, path.c_str() + 1, sizeof(addr.sun_path) - 2);

        socklen_t addr_len = static_cast<socklen_t>(
            offsetof(struct sockaddr_un, sun_path) + strlen(path.c_str() + 1) + 1
        );

        // Создаем сокет
        int fd = assertOK(socket(AF_UNIX, SOCK_STREAM, 0), "socket");

        // Подключаемся к серверу
        assertOK(connect(fd, (struct sockaddr*)&addr, addr_len), "connect");

        // Сохраняем дескрипторы для чтения и записи
        m_fd_reader = fd;
        m_fd_writer = fd;

        setBlockingMode(false);

    } catch (const std::exception& e) {
        finalize();
        throw std::runtime_error(
            std::string("unix socket client initialization error: ") + e.what()
        );
    }
}


void CStreamPortIPCClient::finalize() {
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
} // io
} // iridium


#endif // UNIX_PLATFORM
