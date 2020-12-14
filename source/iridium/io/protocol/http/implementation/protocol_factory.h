// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROTOCOL_FACTORY_C1D59795_4D13_4FA7_A9D7_D11A452EC2D1
#define HEADER_PROTOCOL_FACTORY_C1D59795_4D13_4FA7_A9D7_D11A452EC2D1


#include "iridium/io/protocol/protocol_factory.h"


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {
        
    
//class CResponseProtocolFactory: public IProtocolFactory {
//public:
//    DEFINE_IMPLEMENTATION(CResponseProtocolFactory)
//    CResponseProtocolFactory() = default;
    
//    IProtocolHandler::TSharedPtr    createProtocolHandler() override;
//};


template<typename TProtocolHandler>
class CSimpleProtocolFactory: public IProtocolFactory {
public:
    DEFINE_IMPLEMENTATION(CSimpleProtocolFactory)
    CSimpleProtocolFactory() = default;

    IProtocolHandler::TSharedPtr    createProtocolHandler() override;
};


template<typename TProtocolHandler>
IProtocolHandler::TSharedPtr CSimpleProtocolFactory<TProtocolHandler>::createProtocolHandler() {
    return TProtocolHandler::create(); // ----->
}


} // implementation
} // http
} // protocol
} // io
} // iridium


#endif // HEADER_PROTOCOL_FACTORY_C1D59795_4D13_4FA7_A9D7_D11A452EC2D1
