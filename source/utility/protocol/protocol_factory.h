#ifndef HEADER_PROTOCOL_FACTORY_6E49C5FE_ACE3_453D_B03A_222083C6A96C
#define HEADER_PROTOCOL_FACTORY_6E49C5FE_ACE3_453D_B03A_222083C6A96C


#include "utility/smart_ptr.h"
#include "protocol.h"


namespace utility {
namespace protocol {


class IProtocolFactory {
public:
    DEFINE_INTERFACE(IProtocolFactory)
    IProtocol::TSharedPtr create();
};


} // protocol
} // utility



#endif // HEADER_PROTOCOL_FACTORY_6E49C5FE_ACE3_453D_B03A_222083C6A96C
