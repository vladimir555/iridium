// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_URL_F2F08356_C16A_4981_92C6_32E1B85D4384
#define HEADER_URL_F2F08356_C16A_4981_92C6_32E1B85D4384


#include "iridium/enum.h"
#include "iridium/smart_ptr.h"

#include <memory>
#include <vector>
#include <set>

#include "iridium/convertion/convert.h"
#include "net/types.h"


namespace iridium {
namespace io {


// todo: make URI
class URL {
public:
    DEFINE_ENUM(
        TProtocol, 
        TCP         = -2,
        UDP         = -3,
        WS          = -4,
        WSS         = -5,
        FILE        = -6,
        HTTP        = 80,
        HTTPS       = 443,
        SSH         = 22,
        Telnet      = 23,
        RTSP        = 554,
        MYSQL       = 3306,
        POSTGRES    = 5432
    )

    DEFINE_CREATE(URL)

    URL(std::string const &url);
   ~URL() = default;

//    URL &operator = (URL const &url) = default;

    typedef std::shared_ptr<net::TIPv4 const>   TIPv4SharedPtr;
    typedef std::shared_ptr<net::TPort const>   TPortSharedPtr;
    typedef std::shared_ptr<net::THost const>   THostSharedPtr;
    typedef std::shared_ptr<net::TPath const>   TPathSharedPtr;
    typedef std::shared_ptr<TProtocol  const>   TProtocolSharedPtr;

    TIPv4SharedPtr      const getIPv4()         const;
    std::string         const getIPv4AsString() const;
    TPortSharedPtr      const getPort()         const;
    THostSharedPtr      const getHost()         const;
    TPathSharedPtr      const getPath()         const;
    TProtocol           const getProtocol()     const;
    net::TAddress       const getAddress()      const;

    bool operator == (URL const &url) const;
    bool operator <  (URL const &url) const;
//    ::std::size_t operator ()(const ::std::set<URL*> &vertexSet) const { return 0; }

private:
    TIPv4SharedPtr      m_ipv4;
    TPortSharedPtr      m_port;
    THostSharedPtr      m_host;
    TPathSharedPtr      m_path;
    TProtocolSharedPtr  m_protocol;
    net::TAddress       m_address;
};


} // io
} // iridium


DEFINE_CONVERT(iridium::io::URL, std::string)
DEFINE_CONVERT(std::string, iridium::io::URL)


#endif // HEADER_URL_F2F08356_C16A_4981_92C6_32E1B85D4384
