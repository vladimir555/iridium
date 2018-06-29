#ifndef HEADER_PACKET_1C696E95_2E3D_4ABF_8BF3_BD56613F9368
#define HEADER_PACKET_1C696E95_2E3D_4ABF_8BF3_BD56613F9368


#include "utility/io/protocol/packet.h"


namespace utility {
namespace io {
namespace protocol {
namespace implementation {


class CPacket: public IPacket {
public:
    DEFINE_CREATE(CPacket)
    CPacket(TData const &data, bool const &is_last = false);
    virtual ~CPacket() = default;

    TData getData()     const override;
    bool  getIsLast()   const override;

private:
    TData   m_data;
    bool    m_is_last;
};


} // implementation
} // protocol
} // io
} // utility


#endif // HEADER_PACKET_1C696E95_2E3D_4ABF_8BF3_BD56613F9368
