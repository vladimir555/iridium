#include "peer_handler.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {


void CPeerHandler::initialize() {}


void CPeerHandler::finalize() {}


CPeerHandler::TItems CPeerHandler::handle(TItems const &peers_) {
//    threading::sleep(500);
//    LOGT << "-----in  peers size " << peers_.size();
    TItems peers;
    for (auto const &peer : peers_) {
//        LOGT << "peer fd " << peer->event->stream->getID() << " event " << peer->event->type;

        bool is_continue    = peer->protocol_handler->update(peer->transmitter, peer->event);
        bool is_transmitted = peer->transmitter->transmit(peer->event);

//        LOGT << "is_continue    = " << is_continue;
//        LOGT << "is_transmitted = " << is_transmitted;

        if (is_transmitted) {
//            LOGT << "push by handler, fd " << peer->event->stream->getID() << " event " << peer->event->type;
            peers.push_back(peer);
        } else {
            if (!is_continue) {
                peer->event->type = Event::TType::CLOSE;
//                LOGT << "push close event, fd " << peer->event->stream->getID();
                peers.push_back(peer);
            }
        }
    }

//    LOGT << "-----out peers size " << peers.size();
    return peers; // ----->
}


} // implementation
} // net
} // io
} // iridium
