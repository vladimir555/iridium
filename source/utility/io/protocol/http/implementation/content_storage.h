#ifndef HEADER_CONTENT_STORAGE_C92EBD57_DA63_4A76_8A56_343A9E6FEFCB
#define HEADER_CONTENT_STORAGE_C92EBD57_DA63_4A76_8A56_343A9E6FEFCB


#include "utility/io/protocol/http/content_storage.h"


namespace utility {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


class CContentStorage: public IContentStorage {
public:
    DEFINE_IMPLEMENTATION(CContentStorage)
    CContentStorage(std::string const &root_path);
    fs::IFileStreamReader::TSharedPtr getContent(std::string const &path) override;
    
private:
    std::string m_root_path;
};


} // implementation
} // http
} // protocol
} // io
} // utility


#endif // HEADER_CONTENT_STORAGE_C92EBD57_DA63_4A76_8A56_343A9E6FEFCB
