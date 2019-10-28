#ifndef HEADER_CONTENT_STORAGE_357A6B8F_B845_4480_B73F_8334B28B71A6
#define HEADER_CONTENT_STORAGE_357A6B8F_B845_4480_B73F_8334B28B71A6


#include "iridium/smart_ptr.h"
#include "iridium/io/fs/file_stream.h"
#include "iridium/pattern/initializable.h"
#include <string>


namespace iridium {
namespace io {
namespace protocol {
namespace http {


class IContentStorage {
public:
    DEFINE_INTERFACE(IContentStorage)
    virtual fs::IFileStreamReader::TSharedPtr getContent(std::string const &path) = 0;
};


} // http
} // protocol
} // io
} // iridium


#endif // HEADER_CONTENT_STORAGE_357A6B8F_B845_4480_B73F_8334B28B71A6
