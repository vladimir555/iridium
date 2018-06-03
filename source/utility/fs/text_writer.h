#ifndef HEADER_TEXT_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
#define HEADER_TEXT_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0


#include <string>
#include <stdio.h>

#include "utility/smart_ptr.h"
#include "utility/pattern/initializable.h"
#include "utility/io/stream.h"


namespace utility {
namespace fs {


// todo: Adapter for IStream
class IFileWriter:
    public pattern::IInitializable,
    public io::IStreamWriter
{
public:
    DEFINE_INTERFACE(IFileWriter)
    virtual void flush() = 0;
};


} // fs
} // utility


#endif // HEADER_TEXT_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
