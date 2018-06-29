#ifndef HEADER_SOCKET_HANDLER_CDD6369B_0F52_49EE_ADC4_E3E8F3204981
#define HEADER_SOCKET_HANDLER_CDD6369B_0F52_49EE_ADC4_E3E8F3204981


#include "utility/smart_ptr.h"
#include "socket.h"


namespace utility {
namespace net {


class ISocketHandler {
public:
    DEFINE_INTERFACE(ISocketHandler)

    virtual void handle(ISocket::TEvent::TAction const &action) = 0;
};


} // net
} // utility


#endif // HEADER_SOCKET_HANDLER_CDD6369B_0F52_49EE_ADC4_E3E8F3204981
