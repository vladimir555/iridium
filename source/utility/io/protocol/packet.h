#ifndef HEADER_PACKET_DE2C3ECA_9BFE_4F42_8A06_688AB12D5E54
#define HEADER_PACKET_DE2C3ECA_9BFE_4F42_8A06_688AB12D5E54


#include "utility/smart_ptr.h"
#include "utility/convertion/convert.h"

#include <vector>


namespace utility {
namespace io {
namespace protocol {


class IPacket {
public:
    DEFINE_INTERFACE(IPacket)
    typedef std::vector<uint8_t> TData;

    virtual TData getData()     const = 0;
    virtual bool  getIsLast()   const = 0;
};


} // protocol
} // io
} // utility


DEFINE_CONVERT(utility::io::protocol::IPacket::TData, std::string)
DEFINE_CONVERT(std::string, utility::io::protocol::IPacket::TData)


#endif // HEADER_PACKET_DE2C3ECA_9BFE_4F42_8A06_688AB12D5E54
