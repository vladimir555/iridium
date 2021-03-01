#ifndef HEADER_SERVER_E098601F_354D_4D93_9FF3_2716F3850676
#define HEADER_SERVER_E098601F_354D_4D93_9FF3_2716F3850676


#include "iridium/pattern/initializable.h"
#include "stream.h"
#include "protocol/session.h"


namespace iridium {
namespace io {


// todo: mv to net
class IPeerSessionFactory {
public:
    DEFINE_INTERFACE(IPeerSessionFactory)
    virtual protocol::ISession::TSharedPtr createSession(IStreamPort::TSharedPtr const &peer) = 0;
};


class IServer: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IServer)
};


} // io
} // iridium


#endif // HEADER_SERVER_E098601F_354D_4D93_9FF3_2716F3850676
