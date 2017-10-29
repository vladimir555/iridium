#ifndef HEADER_SOCKET_423B8F1C_F93C_43CC_99D9_06CB1A1062CF
#define HEADER_SOCKET_423B8F1C_F93C_43CC_99D9_06CB1A1062CF


#include "utility/smart_ptr.h"
#include "utility/pattern/initializable.h"
#include "utility/convertion/convert.h"

#include "url.h"
#include "types.h"

#include <vector>
#include <string>
#include <list>


#include <map>
#include "utility/threading/implementation/mutex.h"


namespace utility {
namespace networking {


class ISocketStream {
public:
    DEFINE_SMART_PTR(ISocketStream)
    virtual ~ISocketStream() = default;

    typedef std::vector<uint8_t> TPacket;

    virtual void write(TPacket const &packet) = 0;
    // todo: expected pacet size parameter
    virtual TPacket read() = 0;
    virtual void close() = 0;
    virtual URL getURL() const = 0;

//    static threading::implementation::CMutex    map_url_read_cache_mutex;
//    static std::map<std::string, TPacket>       map_url_read_cache;
};


class ISocket : public ISocketStream {
public:
    DEFINE_SMART_PTR(ISocket)
    virtual ~ISocket() = default;

    typedef std::list<ISocketStream::TSharedPtr> TSocketStreams;

    virtual void open() = 0;
    virtual void connect() = 0;
    virtual void listen() = 0;
    virtual TSocketStreams accept() = 0;
    virtual void interrupt() = 0;
};


} // networking
} // utility


DEFINE_CONVERT(utility::networking::ISocketStream::TPacket, std::string)
DEFINE_CONVERT(std::string, utility::networking::ISocketStream::TPacket)


#endif // HEADER_SOCKET_423B8F1C_F93C_43CC_99D9_06CB1A1062CF
