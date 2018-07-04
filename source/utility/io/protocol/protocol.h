#ifndef HEADER_PROTOCOL_570AE72E_F2A1_4E8F_B673_F4CFD49B3014
#define HEADER_PROTOCOL_570AE72E_F2A1_4E8F_B673_F4CFD49B3014


#include "utility/smart_ptr.h"
#include "utility/io/stream_proxy.h"


namespace utility {
namespace io {
namespace protocol {


class IProtocol: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IProtocol)
    IStreamProxy::TSharedPtr update(IStreamProxy::TSharedPtr const &stream_proxy, IStream::TSharedPtr const &stream);
};


} // protocol
} // io
} // utility


#endif // HEADER_PROTOCOL_570AE72E_F2A1_4E8F_B673_F4CFD49B3014
