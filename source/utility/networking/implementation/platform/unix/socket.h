#include "utility/platform.h"



#ifdef UNIX_PLATFORM


#ifndef HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12
#define HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12


#include "utility/networking/socket.h"
#include "utility/networking/url.h"
#include "utility/encryption/ssl.h"
#include "utility/threading/implementation/recursive_mutex.h"

#include <map>
#include <memory>
#include <atomic>


namespace utility {
namespace networking {
namespace implementation {
namespace platform {
namespace unix {


class CSocket:
    public ISocket,
    public threading::implementation::CRecursiveMutex
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
    int             assertOK(int const &result, std::string const &message) const;
    URL             getPeerURL(int const &socket);
    void            setBlockingMode(bool const &is_blocking);
    std::list<int>  acceptInternal();

    bool            m_is_blocking_mode;
    int             m_socket_fd;
    URL const       m_url;

    encryption::IContext::TSharedPtr    m_encryptor;
    encryption::ISSL::TSharedPtr        m_ssl;

    CSocket::TSharedPtr createInternal(int const &socket_fd);

private:
    void remove(CSocket const * const accepted_socket);

    std::list<CSocket::TSharedPtr>      m_accepted_sockets;
    CSocket                            *m_acceptor;
};


} // unix
} // platform
} // implementation
} // networking
} // utility


#endif // HEADER_SOCKET_397080E6_6910_4C45_AAB7_D404A42C7D12


#endif // UNIX_PLATFORM
