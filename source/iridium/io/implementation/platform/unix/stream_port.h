#ifndef HEADER_STREAM_PORT_3353B98B_8ECE_48F8_8627_EE37A8AB711C
#define HEADER_STREAM_PORT_3353B98B_8ECE_48F8_8627_EE37A8AB711C


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/url.h"
#include "iridium/io/stream.h"
#include "iridium/convertion/convert.h"
#include "iridium/pattern/non_copyable.h"

#include <cstring>
#include <string>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {
namespace unix_ {


class CStreamPort: virtual public IStreamPort, public pattern::NonCopyable {
protected:
    CStreamPort(URL const &url);
    virtual ~CStreamPort() = default;

public:
    virtual void        initialize()                            override = 0;
    virtual void        finalize()                              override = 0;
    size_t              write(Buffer::TSharedPtr const &buffer) override;
    Buffer::TSharedPtr  read(size_t const &size = DEFAULT_BUFFER_SIZE) override;
    int                 getID() const                           override;

protected:
//    void                open();
//    void                close();
    void                setBlockingMode(int const &socket, bool const &is_blocking);

    int             m_fd;
    URL::TSharedPtr m_url;
    bool            m_is_opened;
    bool            m_is_blocking_mode;

protected:
    template<typename T>
    T assertOK(T const &result, std::string const &message);

    static int initSignal();
};


template<typename T>
T CStreamPort::assertOK(T const &result, std::string const &message) {
    using convertion::convert;
    using std::string;
    if (result < 0) {
        if (m_url)
            throw std::runtime_error(message + ": url " + convert<string>(*m_url) +
                ", " + std::strerror(errno) + ", code " + convert<string>(errno)); // ----->
        else
            throw std::runtime_error(message +
                ", " + std::strerror(errno) + ", code " + convert<string>(errno)); // ----->
    } else
        return result; // ----->
}


} // _unix
} // platform
} // implementation
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_STREAM_PORT_3353B98B_8ECE_48F8_8627_EE37A8AB711C
