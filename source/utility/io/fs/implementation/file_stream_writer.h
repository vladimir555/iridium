#ifndef HEADER_FILE_STREAM_WRITER_06F9B4BF_E655_4FB7_8EA0_185306B7C2A3
#define HEADER_FILE_STREAM_WRITER_06F9B4BF_E655_4FB7_8EA0_185306B7C2A3


#include "utility/io/fs/file_stream.h"
#include "file_stream.h"


namespace utility {
namespace io {
namespace fs {
namespace implementation {
    
    
class CFileStreamWriter: public CFileStream, public IFileStreamWriter {
public:
    DEFINE_IMPLEMENTATION(CFileStreamWriter)
    CFileStreamWriter(std::string const &path);
    
    void    initialize()  override;
    void    finalize()    override;
    
    int     getID() const override;
    
    size_t  write(Buffer const &buffer) override;
    void    flush() override;
};
    
    
} // implementation
} // fs
} // io
} // utility


#endif // HEADER_FILE_STREAM_WRITER_06F9B4BF_E655_4FB7_8EA0_185306B7C2A3
