#ifndef HEADER_STREAM_PORT_ACCEPTOR_WINDOWS_0F1A5F2B
#define HEADER_STREAM_PORT_ACCEPTOR_WINDOWS_0F1A5F2B

#include "iridium/platform.h"

#ifdef WINDOWS_PLATFORM

#include "iridium/io/uri.h"
#include "iridium/io/stream.h"
#include "iridium/pattern/non_copyable.h"
#include <mutex>
#include <list>
#include <string>
#include <memory>

namespace iridium::io::implementation::platform {


class CStreamPortAcceptor: public IStreamAcceptor, public pattern::NonCopyable {
public:
    DEFINE_IMPLEMENTATION(CStreamPortAcceptor)

    explicit CStreamPortAcceptor(URI const &uri);

    void initialize() override;
    void finalize() override;

    std::list<uintptr_t> getHandles() const override;
    URI::TSharedPtr getURI() const override;

    std::list<IStreamPort::TSharedPtr> accept() override;
private:
    URI::TSharedPtr m_uri;
};


} // namespace

#endif // WINDOWS_PLATFORM

#endif // HEADER_STREAM_PORT_ACCEPTOR_WINDOWS_0F1A5F2B
