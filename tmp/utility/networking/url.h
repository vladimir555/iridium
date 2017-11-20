#ifndef HEADER_URL_F2F08356_C16A_4981_92C6_32E1B85D4384
#define HEADER_URL_F2F08356_C16A_4981_92C6_32E1B85D4384


#include "utility/enum.h"
#include "utility/smart_ptr.h"

#include <memory>
#include <vector>
#include <set>

#include "utility/convertion/convert.h"
#include "types.h"


namespace utility {
namespace networking {


class URL {
public:
    DEFINE_ENUM(
        TProtocol, 
        TCP         = -2,
        UDP         = -3,
        HTTP        = 80,
        HTTPS       = 443,
        SSH         = 22,
        Telnet      = 23,
        MYSQL       = 3306,
        POSTGRESQL  = 5432
    )

    URL(std::string const &url);
   ~URL() = default;

//    URL &operator = (URL const &url) = default;

    typedef std::shared_ptr<TIPv4 const>        TIPv4SharedPtr;
    typedef std::shared_ptr<TPort const>        TPortSharedPtr;
    typedef std::shared_ptr<THost const>        THostSharedPtr;
    typedef std::shared_ptr<TProtocol const>    TProtocolSharedPtr;

    TIPv4SharedPtr      const getIPv4()         const;
    std::string         const getIPv4AsString() const;
    TPortSharedPtr      const getPort()         const;
    THostSharedPtr      const getHost()         const;
    TProtocol           const getProtocol()     const;
    TAddress            const getAddress()      const;

    bool operator == (URL const &url) const;
    bool operator <  (URL const &url) const;
//    ::std::size_t operator ()(const ::std::set<URL*> &vertexSet) const { return 0; }

private:
    TIPv4SharedPtr      m_ipv4;
    TPortSharedPtr      m_port;
    THostSharedPtr      m_host;
    TProtocolSharedPtr  m_protocol;
    TAddress            m_address;
};


} // networking
} // utility


DEFINE_CONVERT(utility::networking::URL, std::string)
DEFINE_CONVERT(std::string, utility::networking::URL)


#endif // HEADER_URL_F2F08356_C16A_4981_92C6_32E1B85D4384
