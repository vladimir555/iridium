#ifndef HEADER_STREAM_POOL_A80375CE_40E5_4CDA_9916_2F62667809BD
#define HEADER_STREAM_POOL_A80375CE_40E5_4CDA_9916_2F62667809BD


#include "iridium/smart_ptr.h"
#include "stream.h"
#include "protocol/protocol_factory.h"
#include "listener.h"


namespace iridium {
namespace io {


class IStreamPool: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IStreamPool)

    virtual void add(IStreamPort::TSharedPtr const &stream, protocol::IProtocolHandler::TSharedPtr const &protocol_handler) = 0;
    virtual void del(IStream::TSharedPtr     const &stream) = 0;
};


} // io
} // iridium


#endif // HEADER_STREAM_POOL_A80375CE_40E5_4CDA_9916_2F62667809BD
