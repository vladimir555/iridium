// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358
#define HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358


#include "iridium/platform.h"
#include "iridium/smart_ptr.h"
#include "iridium/enum.h"
#include "iridium/pattern/initializable.h"
#include "iridium/convertion/convert.h"

#include "buffer.h"
#include "uri.h"

#include <list>
#include <string>


namespace iridium::io {


static size_t const DEFAULT_BUFFER_SIZE = 16384;


//! \~english @deprecated todo: rm, deprecated
//! \~russian @deprecated todo: rm, deprecated
class IStream: public virtual pattern::IInitializable {
public:
    DEFINE_INTERFACE(IStream)

    DEFINE_ENUM(
        THandleType,
        READER  = 0x01,
        WRITER  = 0x02,
        PID     = 0x04
    );

    using TMapHandleTypeIdent =
        std::unordered_map<THandleType, uintptr_t>;

    virtual TMapHandleTypeIdent
        getHandles() const = 0;
    virtual URI::TSharedPtr
        getURI() const = 0;
};


//! \~english @deprecated todo: rm, deprecated
//! \~russian @deprecated todo: rm, deprecated
class IStreamReader: public virtual IStream {
public:
    DEFINE_INTERFACE(IStreamReader)
    /// return nullptr on EOF or unexpected closed,
    /// empty buffer on not ready data
    /// size = 0 returns last accumulated buffer if exists
    virtual Buffer::TSharedPtr
        read(size_t const &size = DEFAULT_BUFFER_SIZE) = 0;
};


//! \~english @deprecated todo: rm, deprecated
//! \~russian @deprecated todo: rm, deprecated
class IStreamWriter: public virtual IStream {
public:
    DEFINE_INTERFACE(IStreamWriter)
    /// return 0 - socket/fd not ready
    virtual size_t
        write(Buffer::TSharedPtr const &buffer) = 0;
};


//! \~english @deprecated todo: rm, deprecated
//! \~russian @deprecated todo: rm, deprecated
class IStreamPort:
    public virtual IStreamReader,
    public virtual IStreamWriter
{
public:
    DEFINE_INTERFACE(IStreamPort)
};


//! \~english @deprecated todo: rm, deprecated
//! \~russian @deprecated todo: rm, deprecated
class IStreamAcceptor: public virtual IStream {
public:
    DEFINE_INTERFACE(IStreamAcceptor)
    virtual std::list<IStreamPort::TSharedPtr>
        accept() = 0;
};


} // namespace iridium::io


DEFINE_ENUM_CONVERT(iridium::io::IStream::THandleType)
DEFINE_CONVERT(std::string, iridium::io::IStream);
DEFINE_CONVERT(std::string, iridium::io::IStreamReader);
DEFINE_CONVERT(std::string, iridium::io::IStreamWriter);
DEFINE_CONVERT(std::string, iridium::io::IStreamPort);


#endif // HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358
