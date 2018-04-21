#include "socket_handler.h"

#include "utility/protocol/implementation/packet.h"

#include "utility/logging/logger.h"


using utility::convertion::convert;
using std::string;
using utility::protocol::implementation::CPacket;


size_t const DEFAULT_READ_BUFFER_MAX_SIZE = 1024;


namespace utility {
namespace networking {
namespace implementation {


CSocketHandler::CSocketHandler(ISocketStream::TSharedPtr const &socket, protocol::IProtocol::TSharedPtr const &protocol)
:
    m_socket        (socket),
    m_protocol      (protocol),
    m_is_need_close (false)
{
    LOGT << "create socket handler";
}


void CSocketHandler::handle(TAction const &action) {
    LOGT << "socket handler action " << action;

    if (action == TAction::ACCEPT && m_last_action == TAction::UNKNOWN) {
        m_last_action = action;
        return; // ----->
    }

    if (action == TAction::READ && (m_last_action == TAction::ACCEPT || m_last_action == TAction::READ)) {
        m_last_action   = action;

        auto packet     = m_socket->read();
        m_read_buffer.insert(m_read_buffer.end(), packet.begin(), packet.end());

        auto request    = CPacket::create(m_read_buffer);
        auto response   = m_protocol->exchange(request);

        if (response) {
            m_write_buffer      = response->getData();
            m_write_buffer_pos  = m_socket->write(m_write_buffer);
            m_last_action       = TAction::WRITE;
            if (response->getIsLast())
                m_is_need_close = true;
        } else {
            // todo: read timeout limit
            // read buffer limit
            if (m_read_buffer.size() > DEFAULT_READ_BUFFER_MAX_SIZE) {
                throw std::runtime_error(
                    "socket handler error: action " +
                    convert<string>(m_last_action) + " -> " +
                    convert<string>(action) +
                    " read buffer size limit " +
                    convert<string>(m_read_buffer.size()) + " > " +
                    convert<string>(DEFAULT_READ_BUFFER_MAX_SIZE)); // ----->
            }
        }
        return; // ----->
    }

    if (action == TAction::WRITE && m_last_action == TAction::WRITE) {
        m_write_buffer_pos      = m_socket->write(TPacket(&m_write_buffer[m_write_buffer_pos], &m_write_buffer[m_write_buffer.size() - 1]));
        LOGT << "write packet ";// << TPacket(&m_write_buffer[m_write_buffer_pos], &m_write_buffer[m_write_buffer.size() - 1]);
        if (m_write_buffer_pos >= m_write_buffer.size() - 1) {
            m_write_buffer.clear();
            m_write_buffer_pos  = 0;
            if (m_is_need_close) {
                m_socket->close();
                m_last_action   = TAction::CLOSE;
            } else
                m_last_action   = TAction::READ;
        }
        return; // ----->
    }

    if (action == TAction::CLOSE && m_last_action != TAction::CLOSE) {
        m_socket->close();
        return; // ----->
    }

    throw std::runtime_error("socket handler error: action " +
        convert<string>(m_last_action) + " -> " +
        convert<string>(action)); // ----->
}


} // implementation
} // networking
} // utility
