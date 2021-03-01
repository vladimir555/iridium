#ifndef HEADER_SERVER_1B8D7F67_9FB4_42BD_B216_688B1F6A12AC
#define HEADER_SERVER_1B8D7F67_9FB4_42BD_B216_688B1F6A12AC


#include "iridium/io/server.h"
#include "iridium/io/stream_pool.h"
#include "iridium/io/protocol/session.h"
#include "iridium/io/net/socket.h"

#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {


class CServer: public IServer {
public:
    DEFINE_IMPLEMENTATION(CServer)
    CServer(URL const &url, IPeerSessionFactory::TSharedPtr const &session_factory);

    void initialize()   override;
    void finalize()     override;

private:
    URL                             m_url;
    IPeerSessionFactory::TSharedPtr m_session_factory;
    IStreamPool::TSharedPtr         m_stream_pool;
};


} // implementation
} // net
} // io
} // iridium


#endif // HEADER_SERVER_1B8D7F67_9FB4_42BD_B216_688B1F6A12AC
