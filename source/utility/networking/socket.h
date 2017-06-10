#ifndef HEADER_SOCKET_423B8F1C_F93C_43CC_99D9_06CB1A1062CF
#define HEADER_SOCKET_423B8F1C_F93C_43CC_99D9_06CB1A1062CF


#include "utility/smart_ptr.h"
#include "utility/pattern/initializable.h"
#include "utility/convertion/convert.h"

#include <vector>
#include <string>


namespace utility {
namespace networking {


class ISocketStream {
public:
    DEFINE_SMART_PTR(ISocketStream)
    virtual ~ISocketStream() = default;

    typedef std::vector<uint8_t> packet_t;

    virtual void write(packet_t const &packet) = 0;
    // todo: expected pacet size parameter
    virtual packet_t read() = 0;
};


class ISocket : public ISocketStream {
public:
    DEFINE_SMART_PTR(ISocket)
    virtual ~ISocket() = default;

    virtual void open() = 0;
    virtual void close() = 0;
    virtual void connect() = 0;
    virtual void listen() = 0;
    virtual ISocketStream::TSharedPtr accept() = 0;
    virtual void interrupt() = 0;
};


} // networking
} // utility


DEFINE_CONVERT(utility::networking::ISocketStream::packet_t, std::string)
DEFINE_CONVERT(std::string, utility::networking::ISocketStream::packet_t)


#endif // HEADER_SOCKET_423B8F1C_F93C_43CC_99D9_06CB1A1062CF
