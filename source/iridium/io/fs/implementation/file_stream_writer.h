// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FILE_STREAM_WRITER_06F9B4BF_E655_4FB7_8EA0_185306B7C2A3
#define HEADER_FILE_STREAM_WRITER_06F9B4BF_E655_4FB7_8EA0_185306B7C2A3


#include "iridium/io/fs/file_stream.h"
#include "file_stream.h"


namespace iridium {
namespace io {
namespace fs {
namespace implementation {
    
    
class CFileStreamWriter: public CFileStream, public IFileStreamWriter {
public:
    DEFINE_IMPLEMENTATION(CFileStreamWriter)
    CFileStreamWriter(std::string const &path, bool const &is_rewrite = false);
    
    void    initialize()  override;
    void    finalize()    override;
    
    TID     getID() const override;
    URI::TSharedPtr getURI() const override;
    
    size_t  write(Buffer::TSharedPtr const &buffer) override;
    void    flush() override;
};
    
    
} // implementation
} // fs
} // io
} // iridium


#endif // HEADER_FILE_STREAM_WRITER_06F9B4BF_E655_4FB7_8EA0_185306B7C2A3
