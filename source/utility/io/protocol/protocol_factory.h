#ifndef HEADER_PROTOCOL_FACTORY_6E49C5FE_ACE3_453D_B03A_222083C6A96C
#define HEADER_PROTOCOL_FACTORY_6E49C5FE_ACE3_453D_B03A_222083C6A96C


#include "utility/smart_ptr.h"
#include "utility/io/stream.h"
#include "utility/io/stream_handler.h"


namespace utility {
namespace io {
namespace protocol {


class IProtocolFactory {
public:
    DEFINE_INTERFACE(IProtocolFactory)
    virtual IStreamHandler::TSharedPtr createStreamHandler(IStream::TSharedPtr const &stream) = 0;
};


} // protocol
} // io
} // utility


#endif // HEADER_PROTOCOL_FACTORY_6E49C5FE_ACE3_453D_B03A_222083C6A96C
