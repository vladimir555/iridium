#ifndef HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
#define HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0


#include "utility/pattern/initializable.h"
#include "utility/io/stream.h"


namespace utility {
namespace io {
namespace fs {


class IFileWriter:
    public IStreamWriter,
    public pattern::IInitializable
{
public:
    DEFINE_INTERFACE(IFileWriter)
    virtual void flush() = 0;
};


class IFileReader:
    public IStreamReader,
    public pattern::IInitializable
{
public:
    DEFINE_INTERFACE(IFileReader)
};


//class IFileStream:
//    public virtual IFileWriter,
//    public virtual IFileReader,
//    public virtual io::IStream
//{
//public:
//    DEFINE_INTERFACE(IFileStream)
//};


} // fs
} // io
} // utility


#endif // HEADER_FILE_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
