#include "utility/platform.h"



#ifdef UNIX_PLATFORM


#ifndef HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12
#define HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12


#include "utility/networking/socket.h"
#include "utility/networking/url.h"
#include "utility/encryption/ssl.h"
#include "utility/threading/implementation/mutex.h"

#include <map>
#include <memory>


namespace utility {
namespace networking {
namespace implementation {
namespace platform {
namespace unix {


class CSocket:
    public ISocket,
    public threading::implementation::CMutex,
    public std::enable_shared_from_this<CSocket>
{
public:
    DEFINE_SMART_PTR(CSocket)
    DEFINE_CREATE(CSocket)

    CSocket(URL const &url);
    virtual ~CSocket();

    void            open() override;
    void            close() override;
    void            write(TPacket const &packet) override;
    TPacket         read() override;
    void            listen() override;
    TSocketStreams  accept() override;
    void            interrupt() override;
    void            connect() override;
    URL             getURL() const override;

protected:
    CSocket(int const &socket, URL const &url, unix::CSocket::TSharedPtr const &acceptor);
    int             assertOK(int const &result, std::string const &message) const;
    URL             getPeerURL(int const &socket);
    void            setBlockingMode(bool const &is_blocking);
    std::list<int>  acceptInternal();

    bool            m_is_blocking_mode;
    int             m_socket_fd;
    URL             m_url;

    encryption::IContext::TSharedPtr    m_encryptor;
    encryption::ISSL::TSharedPtr        m_ssl;

private:
    void            updateAcceptedSocketFD(int const &socket_fd);
    void            removeURLFromMap(URL const &url);

    std::map<URL, CSocket::TSharedPtr>  m_map_url_socket;
    CSocket::TSharedPtr                 m_acceptor;
};


} // unix
} // platform
} // implementation
} // networking
} // utility


#endif // HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12


#endif // UNIX_PLATFORM
