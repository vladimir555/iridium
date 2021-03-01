#ifndef HEADER_STREAM_POOL_A80375CE_40E5_4CDA_9916_2F62667809BD
#define HEADER_STREAM_POOL_A80375CE_40E5_4CDA_9916_2F62667809BD


#include "stream.h"
#include "protocol/session.h"
#include "url.h"
#include "listener.h"


namespace iridium {
namespace io {


class IStreamPool: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IStreamPool)

    // can be called many times, starting session
    virtual void add(protocol::ISession::TSharedPtr const &session) = 0;
    virtual void del(protocol::ISession::TSharedPtr const &session) = 0;
};


} // io
} // iridium


#endif // HEADER_STREAM_POOL_A80375CE_40E5_4CDA_9916_2F62667809BD
