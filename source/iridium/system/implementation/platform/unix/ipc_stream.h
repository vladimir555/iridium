// Файл: shared_memory_stream_port.h
#ifndef HEADER_SHARED_MEMORY_STREAM_PORT_6B6C3504_F50F_4B5E_AC0E_A0363F259C2F
#define HEADER_SHARED_MEMORY_STREAM_PORT_6B6C3504_F50F_4B5E_AC0E_A0363F259C2F


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/uri.h"
#include "iridium/io/implementation/stream_port.h"
#include "iridium/convertion/convert.h"
#include "iridium/pattern/non_copyable.h"

#include <string>
#include <cstring>
#include <atomic>
#include <list>
#include <set>


namespace iridium {
namespace system {
namespace implementation {
namespace platform {
namespace unix_ {


class CIPCStream: public io::implementation::CStreamPort {
public:
    DEFINE_CREATE(CIPCStream)
    CIPCStream(io::URI const &uri, bool const &is_server = false);
    virtual ~CIPCStream() = default;

    void    initialize() override;
    void    finalize() override;

private:
    template<typename TResult>
    TResult assertOK(TResult const &&result, std::string const &message);

    bool        m_is_server;
};


template<typename TResult>
TResult CIPCStream::assertOK(TResult const &&result, std::string const &message) {
    using convertion::convert;
    using std::string;
    if (reinterpret_cast<int>(result) < 0) {
        if (m_uri)
            throw std::runtime_error(message + ": uri " + convert<string>(*m_uri) +
                ", " + std::strerror(errno) + ", code " + convert<string>(errno));
        else
            throw std::runtime_error(message +
                ", " + std::strerror(errno) + ", code " + convert<string>(errno));
    }
    return result;
}


} // unix_
} // platform
} // implementation
} // system
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_SHARED_MEMORY_STREAM_PORT_6B6C3504_F50F_4B5E_AC0E_A0363F259C2F
