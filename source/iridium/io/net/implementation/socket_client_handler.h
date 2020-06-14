#ifndef HEADER_SOCKET_CLIENT_HANDLER_6CA1D0B2_38FA_4894_BD1B_024442AF70C9
#define HEADER_SOCKET_CLIENT_HANDLER_6CA1D0B2_38FA_4894_BD1B_024442AF70C9


#include "iridium/smart_ptr.h"
#include "iridium/threading/runnable.h"
#include "iridium/io/protocol/protocol_factory.h"
#include "iridium/io/net/url.h"
#include "iridium/io/net/socket.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {


class CSocketClientHandler: public threading::IRunnable {
public:
    DEFINE_IMPLEMENTATION(CSocketClientHandler)

    CSocketClientHandler(
        URL const &url,
        protocol::IProtocolHandler::TSharedPtr
            const &protocol_handler
    );

    void run(std::atomic<bool> &is_running) override;

    void initialize()   override;
    void finalize()     override;

private:
    protocol::IProtocolHandler::TSharedPtr
                                m_protocol_handler;
    IListener::TSharedPtr       m_listener;
    ISocket::TSharedPtr         m_socket;
    ITransmitter::TSharedPtr    m_transmitter;
};


} // implementation
} // net
} // io
} // iridium


#endif // HEADER_SOCKET_CLIENT_HANDLER_6CA1D0B2_38FA_4894_BD1B_024442AF70C9
