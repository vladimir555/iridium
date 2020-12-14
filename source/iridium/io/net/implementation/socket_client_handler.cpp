#include "socket_client_handler.h"

#include "iridium/io/implementation/listener.h"
#include "iridium/io/implementation/transmitter.h"

#include "socket.h"
#include "iridium/assert.h"

#include "iridium/logging/logger.h"


using iridium::io::implementation::CListener;
using iridium::io::implementation::CTransmitter;


namespace iridium {
namespace io {
namespace net {
namespace implementation {


//CSocketClientHandler::CSocketClientHandler(
//    URL const &url,
//    protocol::IProtocolHandler::TSharedPtr
//        const &protocol_handler)
//:
//    m_protocol_handler  (protocol_handler),
//    m_listener          (CListener::create()),
//    m_socket            (io::net::implementation::platform::unix::CSocketClient::create(url)),
//    m_transmitter       (CTransmitter::create(m_listener))
//{}


//void CSocketClientHandler::run(std::atomic<bool> &is_running) {
//    bool is_continue    = false;
//    bool is_transmitted = false;

//    {
//        auto initial_event  = Event::create(Event{Event::TType::WRITE, m_socket});
//        is_continue         = m_protocol_handler->redirectStreams(m_transmitter, initial_event);
////        initial_event->type = Event::TType::WRITE;
//        is_transmitted      = m_transmitter->transmit(initial_event);
//    }

//    while (is_running) {

//        for (auto const &event: m_listener->wait()) {
//            LOGT << event->stream->getID() << " " << event->type;
//            is_continue     = m_protocol_handler->redirectStreams(m_transmitter, event);
//            is_transmitted  = m_transmitter->transmit(event);
//        }
//    }
//}


//void CSocketClientHandler::initialize() {
//    m_listener->initialize();
//    m_socket->initialize();
//}


//void CSocketClientHandler::finalize() {
//    m_socket->finalize();
//    m_listener->finalize();
//}


} // implementation
} // net
} // io
} // iridium
