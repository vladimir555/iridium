#ifndef HEADER_PROTOCOL_570AE72E_F2A1_4E8F_B673_F4CFD49B3014
#define HEADER_PROTOCOL_570AE72E_F2A1_4E8F_B673_F4CFD49B3014


#include "utility/pattern/prototype.h"
#include "utility/smart_ptr.h"
#include "packet.h"


namespace utility {
namespace protocol {


class IProtocol /*: public pattern::IPrototype<IProtocol>*/ {
public:
    DEFINE_INTERFACE(IProtocol)

    virtual IPacket::TSharedPtr exchange(IPacket::TSharedPtr const &request) = 0;
};


} // protocol
} // utility


#endif // HEADER_PROTOCOL_570AE72E_F2A1_4E8F_B673_F4CFD49B3014
