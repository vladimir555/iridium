// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_FACTORY_E90EE5D7_FE8C_4512_B392_C50C72BF1662
#define HEADER_STREAM_FACTORY_E90EE5D7_FE8C_4512_B392_C50C72BF1662


#include "iridium/io/stream.h"
#include "iridium/io/url.h"


namespace iridium {
namespace io {


class IStreamFactory {
public:
    DEFINE_INTERFACE(IStreamFactory)
    virtual IStreamPort::TSharedPtr createClientSocket(URL const &url) = 0;
    virtual IStreamPort::TSharedPtr createServerSocket(URL const &url) = 0;
};


} // io
} // iridium


#endif // HEADER_STREAM_FACTORY_E90EE5D7_FE8C_4512_B392_C50C72BF1662
