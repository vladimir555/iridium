// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONTENT_STORAGE_C92EBD57_DA63_4A76_8A56_343A9E6FEFCB
#define HEADER_CONTENT_STORAGE_C92EBD57_DA63_4A76_8A56_343A9E6FEFCB


#include "iridium/io/protocol/http/content_storage.h"


namespace iridium {
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
} // iridium


#endif // HEADER_CONTENT_STORAGE_C92EBD57_DA63_4A76_8A56_343A9E6FEFCB
