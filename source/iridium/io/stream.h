// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358
#define HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358


#include "iridium/platform.h"
#include "iridium/smart_ptr.h"
#include "iridium/pattern/initializable.h"

#include "buffer.h"
#include "uri.h"

#include <list>
//#include <functional>


//#ifdef WINDOWS_PLATFORM
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//#endif


namespace iridium {
namespace io {


static size_t const DEFAULT_BUFFER_SIZE = 65535;


class IStream: public virtual pattern::IInitializable {
public:
    DEFINE_INTERFACE(IStream)
    virtual std::list<uintptr_t>    getHandles()    const = 0;
    virtual URI::TSharedPtr         getURI()        const = 0;
};

    
class IStreamReader: public virtual IStream {
public:
    DEFINE_INTERFACE(IStreamReader)
    /// return nullptr on EOF or unexpected closed, empty buffer on not ready data
    /// size = 0 returns last accumulated buffer if exists
    virtual Buffer::TSharedPtr read(size_t const &size = DEFAULT_BUFFER_SIZE) = 0;
};


class IStreamWriter: public virtual IStream {
public:
    DEFINE_INTERFACE(IStreamWriter)
    /// return 0 - socket not ready
    virtual size_t write(Buffer::TSharedPtr const &buffer) = 0;
};

    
class IStreamPort:
    public virtual IStreamReader,
    public virtual IStreamWriter
{
public:
    DEFINE_INTERFACE(IStreamPort)
};


//bool operator < (IStream::TSharedPtr        const &l, IStream::TSharedPtr       const &r);
//bool operator < (IStream::TSharedPtr        const &l, IStream::TConstSharedPtr  const &r);
//bool operator < (IStream::TSharedPtr        const &l, IStream::TWeakPtr         const &r);
//bool operator < (IStream::TSharedPtr        const &l, IStream::TConstWeakPtr    const &r);
//bool operator < (IStream::TConstSharedPtr   const &l, IStream::TSharedPtr       const &r);
//bool operator < (IStream::TConstSharedPtr   const &l, IStream::TConstSharedPtr  const &r);
//bool operator < (IStream::TConstSharedPtr   const &l, IStream::TWeakPtr         const &r);
//bool operator < (IStream::TConstSharedPtr   const &l, IStream::TConstWeakPtr    const &r);
//bool operator < (IStream::TWeakPtr          const &l, IStream::TSharedPtr       const &r);
//bool operator < (IStream::TWeakPtr          const &l, IStream::TConstSharedPtr  const &r);
//bool operator < (IStream::TWeakPtr          const &l, IStream::TWeakPtr         const &r);
//bool operator < (IStream::TWeakPtr          const &l, IStream::TConstWeakPtr    const &r);
//bool operator < (IStream::TConstWeakPtr     const &l, IStream::TSharedPtr       const &r);
//bool operator < (IStream::TConstWeakPtr     const &l, IStream::TConstSharedPtr  const &r);
//bool operator < (IStream::TConstWeakPtr     const &l, IStream::TWeakPtr         const &r);
//bool operator < (IStream::TConstWeakPtr     const &l, IStream::TConstWeakPtr    const &r);


} // io
} // iridium


//template <>
//struct std::hash<iridium::io::IStream::TSharedPtr> {
//    std::size_t operator()(iridium::io::IStream::TSharedPtr const &stream) const;
//};


#endif // HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358
