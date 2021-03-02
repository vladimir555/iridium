#ifndef HEADER_PEER_SESSION_FACTORY_FE0D4A00_72F4_42C0_9B34_B8E3250EFF69
#define HEADER_PEER_SESSION_FACTORY_FE0D4A00_72F4_42C0_9B34_B8E3250EFF69


#include "session.h"


namespace iridium {
namespace io {
namespace protocol {


class IPeerSessionFactory {
public:
    DEFINE_INTERFACE(IPeerSessionFactory)
    virtual ISession::TSharedPtr createSession(IStreamPort::TSharedPtr const &peer) = 0;
};


} // protocol
} // io
} // iridium


#endif // HEADER_PEER_SESSION_FACTORY_FE0D4A00_72F4_42C0_9B34_B8E3250EFF69
