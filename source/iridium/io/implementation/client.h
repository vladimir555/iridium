#ifndef HEADER_CLIENT_B9ECC183_EEA1_45FA_80E2_FA88C07AE6BC
#define HEADER_CLIENT_B9ECC183_EEA1_45FA_80E2_FA88C07AE6BC


#include "iridium/io/client.h"
#include "iridium/io/url.h"
#include "iridium/io/stream_pool.h"


namespace iridium {
namespace io {
namespace implementation {


class CClient: public IClient {
public:
    DEFINE_IMPLEMENTATION(CClient)
    CClient(protocol::ISession::TSharedPtr const &session);
    void initialize()   override;
    void finalize()     override;
private:
    protocol::ISession::TSharedPtr  m_session;
    IStreamPool::TSharedPtr         m_stream_pool;
};


} // implementation
} // io
} // iridium


#endif // HEADER_CLIENT_B9ECC183_EEA1_45FA_80E2_FA88C07AE6BC
