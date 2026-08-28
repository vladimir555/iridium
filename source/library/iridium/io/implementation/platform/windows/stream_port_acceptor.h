#ifndef HEADER_STREAM_PORT_ACCEPTOR_WINDOWS_0F1A5F2B
#define HEADER_STREAM_PORT_ACCEPTOR_WINDOWS_0F1A5F2B

// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "iridium/platform.h"

#ifdef WINDOWS_PLATFORM

#include "iridium/io/uri.h"
#include "iridium/io/stream.h"
#include "iridium/dp/non_copyable.h"
#include <mutex>
#include <list>
#include <string>
#include <memory>

namespace iridium::io::implementation::platform {


class CStreamPortAcceptor: public IStreamAcceptor, public dp::NonCopyable {
public:
    DEFINE_IMPLEMENTATION(CStreamPortAcceptor)

    explicit CStreamPortAcceptor(URI const &uri);

    void initialize() override;
    void finalize() override;

    TMapHandleTypeIdent
        getHandles() const override;
    URI::TSharedPtr
        getURI() const override;
    std::list<IStreamPort::TSharedPtr>
        accept() override;
private:
    URI::TSharedPtr m_uri;
};


} // namespace

#endif // WINDOWS_PLATFORM

#endif // HEADER_STREAM_PORT_ACCEPTOR_WINDOWS_0F1A5F2B
