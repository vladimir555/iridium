// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_PORT_3353B98B_8ECE_48F8_8627_EE37A8AB711C
#define HEADER_STREAM_PORT_3353B98B_8ECE_48F8_8627_EE37A8AB711C


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/uri.h"
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
    CStreamPort(URI const &uri);
    virtual ~CStreamPort() = default;

public:
    virtual void            initialize() override = 0;
    virtual void            finalize() override = 0;
    size_t                  write(Buffer::TSharedPtr const &buffer) override;
    Buffer::TSharedPtr      read(size_t const &size = DEFAULT_BUFFER_SIZE) override;
    std::list<uintptr_t>    getHandles() const override;
    URI::TSharedPtr         getURI() const override;

protected:
    void setBlockingMode(bool const &is_blocking);
    void closeFDs();

    template<typename T>
    T assertOK(T const &result, std::string const &message);

    static int initSignal();

    static int const    YES;

    std::atomic<int>    m_fd_reader;
    std::atomic<int>    m_fd_writer;
    URI::TSharedPtr     m_uri;
    bool                m_is_opened;
    bool                m_is_blocking_mode;

};


template<typename T>
T CStreamPort::assertOK(T const &result, std::string const &message) {
    using convertion::convert;
    using std::string;
    if (result < 0) {
        if (m_uri)
            throw std::runtime_error(message + ": uri " + convert<string>(*m_uri) +
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
