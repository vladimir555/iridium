#ifndef HEADER_PROTOCOL_570AE72E_F2A1_4E8F_B673_F4CFD49B3014
#define HEADER_PROTOCOL_570AE72E_F2A1_4E8F_B673_F4CFD49B3014


#include "utility/pattern/prototype.h"
#include "utility/smart_ptr.h"
#include "packet.h"
#include "utility/io/stream.h"


namespace utility {
namespace io {
namespace protocol {


class IProtocol {
public:
    DEFINE_INTERFACE(IProtocol)
    // returns null on ending processing
    virtual io::IStreamReader::TSharedPtr exchange(io::TBuffer const &request) = 0;
};
// todo: command pattern: ... command socket read -> fs, command write to fs


} // protocol
} // io
} // utility


#endif // HEADER_PROTOCOL_570AE72E_F2A1_4E8F_B673_F4CFD49B3014
