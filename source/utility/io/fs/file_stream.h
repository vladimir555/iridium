#ifndef HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
#define HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0


#include "utility/io/stream.h"


namespace utility {
namespace io {
namespace fs {


class IFileStreamWriter: public IStreamWriter {
public:
    DEFINE_INTERFACE(IFileStreamWriter)
    virtual void flush() = 0;
};


class IFileStreamReader: public IStreamReader {
public:
    DEFINE_INTERFACE(IFileStreamReader)
    virtual size_t getSize() = 0;
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
} // utility


#endif // HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
