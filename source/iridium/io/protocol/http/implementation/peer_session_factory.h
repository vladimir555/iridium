#ifndef HEADER_PEER_FACTORY_548B1B7F_35BE_4BDD_9586_02D2B6C28428
#define HEADER_PEER_FACTORY_548B1B7F_35BE_4BDD_9586_02D2B6C28428


#include "iridium/io/protocol/peer_session_factory.h"


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


class CHTTPPeerSessionFactory: public IPeerSessionFactory {
public:
    DEFINE_IMPLEMENTATION(CHTTPPeerSessionFactory)
    CHTTPPeerSessionFactory() = default;
    ISession::TSharedPtr createSession(IStreamPort::TSharedPtr const &peer) override;
};


} // implementation
} // http
} // protocol
} // io
} // iridium


#endif // HEADER_PEER_FACTORY_548B1B7F_35BE_4BDD_9586_02D2B6C28428
