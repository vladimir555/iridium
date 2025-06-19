#ifndef HEADER_STREAM_PORT_ACCEPTOR_A42C26B0_AEFA_4B20_8793_0FE647A6153E
#define HEADER_STREAM_PORT_ACCEPTOR_A42C26B0_AEFA_4B20_8793_0FE647A6153E


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/uri.h"
#include "iridium/io//stream.h"
#include "iridium/convertion/convert.h"
#include "iridium/pattern/non_copyable.h"

#include <string>
#include <cstring>
#include <atomic>
#include <list>
#include <set>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {
namespace unix_ {


class CStreamPortAcceptor: public IStreamAcceptor {
public:
    DEFINE_IMPLEMENTATION(CStreamPortAcceptor)
    CStreamPortAcceptor(URI const &uri);

    void initialize() override;
    void finalize()   override;

    std::list<uintptr_t> getHandles() const override;
    URI::TSharedPtr      getURI() const override;

    std::list<IStreamPort::TSharedPtr> accept() override;

private:
    template<typename T>
    T assertOK(T const &result, std::string const &message);

    void setBlockingMode(bool const &is_blocking);

    URI::TSharedPtr m_uri;
    int             m_fd;
};


template<typename T>
T CStreamPortAcceptor::assertOK(T const &result, std::string const &message) {
    using convertion::convert;
    using std::string;
    if (result < 0) {
        if (m_uri)
            throw std::system_error(
                errno, std::system_category(),
                message + ": uri " + convert<string>(*m_uri) +
                ", " + std::strerror(errno) + ", code " + convert<string>(errno)); // ----->
        else
            throw std::system_error(
                errno, std::system_category(),
                message +
                ", " + std::strerror(errno) + ", code " + convert<string>(errno)); // ----->
    } else
        return result; // ----->
}


} // unix
} // platform
} // implementation
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_STREAM_PORT_ACCEPTOR_A42C26B0_AEFA_4B20_8793_0FE647A6153E
