#ifndef HEADER_SOCKET_HANDLER_36FE0D79_C5EF_47DD_8394_219A31904066
#define HEADER_SOCKET_HANDLER_36FE0D79_C5EF_47DD_8394_219A31904066


#include "utility/networking/socket_handler.h"
#include "utility/protocol/protocol.h"
#include "utility/protocol/packet.h"
#include "socket.h"


namespace utility {
namespace networking {
namespace implementation {


class CSocketHandler: public ISocketHandler {
public:
    DEFINE_CREATE(CSocketHandler)

    typedef ISocket::TEvent::TAction TAction;

    CSocketHandler(ISocketStream::TSharedPtr const &socket, protocol::IProtocol::TSharedPtr const &protocol);
    virtual ~CSocketHandler() = default;

    void handle(TAction const &action) override;

private:
    ISocketStream::TSharedPtr       m_socket;
    protocol::IProtocol::TSharedPtr m_protocol;
    protocol::IPacket::TData        m_read_buffer;
    protocol::IPacket::TData        m_write_buffer;
    size_t                          m_write_buffer_pos;
    TAction                         m_last_action;
    bool                            m_is_need_close;
};


} // implementation
} // networking
} // utility


#endif // HEADER_SOCKET_HANDLER_36FE0D79_C5EF_47DD_8394_219A31904066
