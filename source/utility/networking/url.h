#ifndef HEADER_URL_F2F08356_C16A_4981_92C6_32E1B85D4384
#define HEADER_URL_F2F08356_C16A_4981_92C6_32E1B85D4384


#include "utility/enum.h"
#include "utility/smart_ptr.h"

#include <memory>
#include <vector>

#include "utility/convertion/convert.h"


namespace utility {
namespace networking {


class URL {
public:
#include "utility/macros/disable_warnings.h"
    DEFINE_ENUM(
        TProtocol, 
        TCP     = -2,
        UDP     = -3,
        HTTP    = 80, 
        HTTPS   = 443, 
        SSH     = 22, 
        Telnet  = 23,
        MYSQL   = 3306
    )
#include "utility/macros/enable_warnings.h"

    explicit URL(std::string const &url);
    ~URL() = default;

    URL &operator = (URL const &url) = default;

    typedef std::vector<uint8_t>    TIPv4;
    typedef uint16_t                TPort;
    typedef std::string             THost;
    typedef std::string             TAddress;

    typedef std::shared_ptr<TIPv4 const>        TIPv4SharedPtr;
    typedef std::shared_ptr<TPort const>        TPortSharedPtr;
    typedef std::shared_ptr<THost const>        THostSharedPtr;
    typedef std::shared_ptr<TProtocol const>    TProtocolSharedPtr;

    TIPv4SharedPtr      const getIPv4()         const;
    std::string         const getIPv4AsString() const;
    TPortSharedPtr      const getPort()         const;
    THostSharedPtr      const getHost()         const;
    TProtocolSharedPtr  const getProtocol()     const;
    TAddress            const getAddress()      const;

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
