// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_URI_F2F08356_C16A_4981_92C6_32E1B85D4384
#define HEADER_URI_F2F08356_C16A_4981_92C6_32E1B85D4384


#include "iridium/enum.h"
#include "iridium/smart_ptr.h"

#include <memory>
#include <vector>
#include <array>
#include <unordered_map>
#include <set>

#include "iridium/convertion/convert.h"


namespace iridium {
namespace io {


//     [IPC]://[/usr/bin]/[ls]                                  - protocol + path
//    [FILE]://[/usr/bin]/[ls]                                  - protocol + path + host
// [PROCESS]://[/usr/bin]/[ls] [dir1] [dir2]                    - protocol + path + host + arguments
//   [HTTPS]://[hostname.com][/dir1/dir2][?arg1=1&arg2=2]       - protocol + host + path + arguments
//   [HTTPS]://[hostname.com][:80][/dir1/dir2][?arg1=1&arg2=2]  - protocol + host + port + path + arguments
//   [MYSQL]://[user:password]@[hostname.com][:80][/db_name]    - protocol + user + password + host + port + path

// host + port + path   = address
// host + path          = address
// path + host          = address

class URI {
public:
    DEFINE_ENUM(
        TProtocol, 
        TCP         = -2,
        UDP         = -3,
        WS          = -4,
        WSS         = -5,
        FILE        = -6,
        PROCESS     = -7,
        IPC         = -8, // unix:// or windows named pipe
        HTTP        = 80,
        HTTPS       = 443,
        SSH         = 22,
        Telnet      = 23,
        RTSP        = 554,
        MYSQL       = 3306,
        POSTGRES    = 5432
    )

    DEFINE_CREATE(URI)

    URI(std::string const &source);
   ~URI() = default;

    struct TIPv4: public std::array<uint8_t , 4> {
        TIPv4();
        DEFINE_CREATE(TIPv4)
    };
    struct TIPv6: public std::array<uint16_t, 8> {
        DEFINE_CREATE(TIPv6)
    };
    
    typedef uint16_t
                TPort;
    typedef std::unordered_map<std::string, std::string>
                TMapNameValue;

    TIPv4::TConstSharedPtr
                getIPv4()       const;
    TIPv6::TConstSharedPtr
                getIPv6()       const;

    TProtocol   getProtocol()   const;
    std::string getUser()       const;
    std::string getPassword()   const;
    std::string getHost()       const;
    TPort       getPort()       const;
    std::string getPath()       const;
    // todo: map
    std::string getArguments()  const;
    std::string getAddress()    const;
    std::string getSource()     const;
    // todo: url arguments

    bool operator == (URI const &uri) const;
    bool operator <  (URI const &uri) const;

private:
    std::string     m_source;
    TProtocol       m_protocol;
    std::string     m_user;
    std::string     m_password;
    std::string     m_host;
    std::string     m_path;
    std::string     m_address;
    std::string     m_arguments;
    std::shared_ptr<TIPv4> mutable m_ipv4;
    std::shared_ptr<TIPv6> mutable m_ipv6;
    TPort           m_port;
};


} // io
} // iridium


DEFINE_ENUM_CONVERT(iridium::io::URI::TProtocol)

DEFINE_CONVERT(std::string, iridium::io::URI::TIPv4)
DEFINE_CONVERT(iridium::io::URI::TIPv4, std::string)

DEFINE_CONVERT(std::string, iridium::io::URI)
DEFINE_CONVERT(iridium::io::URI, std::string)


#endif // HEADER_URI_F2F08356_C16A_4981_92C6_32E1B85D4384
