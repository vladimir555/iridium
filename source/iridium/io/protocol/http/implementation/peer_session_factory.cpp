#include "peer_session_factory.h"


#include "peer_session.h"


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


ISession::TSharedPtr CHTTPPeerSessionFactory::createSession(IStreamPort::TSharedPtr const &peer) {
    return CPeerSession::create(peer); // ----->
}


} // implementation
} // http
} // protocol
} // io
} // iridium
