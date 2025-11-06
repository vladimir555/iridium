// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
#define HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0


#include "iridium/io/stream.h"
#include "iridium/io/fs/file_status.h"


namespace iridium {
namespace io {
namespace fs {


class IFileStream: public virtual IStream {
public:
    DEFINE_INTERFACE(IFileStream)
    virtual TFileStatus getStatus() const = 0;
};


class IFileStreamWriter: public virtual IFileStream, public virtual IStreamWriter {
public:
    DEFINE_INTERFACE(IFileStreamWriter)
    virtual void flush() = 0;
};


class IFileStreamReader: public virtual IFileStream, public virtual IStreamReader {
public:
    DEFINE_INTERFACE(IFileStreamReader)
};


// todo: maybe
//class IFileStreamPort: public IFileStreamReader, public IFileStreamWriter {
//public:
//    virtual void seek(...) = 0;
//}


} // fs
} // io
} // iridium


#endif // HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
