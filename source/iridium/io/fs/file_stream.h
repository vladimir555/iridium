// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
#define HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0


#include "iridium/io/stream.h"

#include <chrono>


namespace iridium {
namespace io {
namespace fs {


// todo: refactoring interfaces


struct TFileStatus {
    std::chrono::system_clock::time_point last_modified;
};


class IFileStreamWriter: public IStreamWriter {
public:
    DEFINE_INTERFACE(IFileStreamWriter)
    virtual void flush() = 0;
};


class IFileStreamReader: public IStreamReader {
public:
    DEFINE_INTERFACE(IFileStreamReader)
    virtual size_t getSize() = 0;
    // todo: move to IFileStream
    virtual TFileStatus getStatus() = 0;
};
    
    
//class IFileStream:
//    public IStream,
//    public IFileStreamReader,
//    public IFileStreamWriter
//{
//public:
//    DEFINE_INTERFACE(IFileStream)
//};


} // fs
} // io
} // iridium


#endif // HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
