#ifndef HEADER_PROTOCOL_FACTORY_C1D59795_4D13_4FA7_A9D7_D11A452EC2D1
#define HEADER_PROTOCOL_FACTORY_C1D59795_4D13_4FA7_A9D7_D11A452EC2D1


#include "iridium/io/protocol/protocol_factory.h"


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {
        
    
class CProtocolFactory: public IProtocolFactory {
public:
    DEFINE_IMPLEMENTATION(CProtocolFactory)
    CProtocolFactory() = default;
    
    IStreamHandler::TSharedPtr createStreamHandler(IStream::TSharedPtr const &stream) override;
};
    

} // implementation
} // http
} // protocol
} // io
} // iridium


#endif // HEADER_PROTOCOL_FACTORY_C1D59795_4D13_4FA7_A9D7_D11A452EC2D1
