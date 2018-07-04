#ifndef HEADER_FILE_STREAM_READER_2BC903C8_78B1_420B_A0B8_30FFF66CC371
#define HEADER_FILE_STREAM_READER_2BC903C8_78B1_420B_A0B8_30FFF66CC371


#include "utility/io/fs/file_stream.h"
#include "file_stream.h"


namespace utility {
namespace io {
namespace fs {
namespace implementation {


class CFileStreamReader: public CFileStream, public IFileStreamReader {
public:
    DEFINE_IMPLEMENTATION(CFileStreamReader)
    CFileStreamReader(std::string const &path);
    
    void    initialize()  override;
    void    finalize()    override;

    int     getID() const override;

    Buffer  read(size_t const &size) override;
    size_t  getSize() override;
};


} // implementation
} // fs
} // io
} // utility


#endif // HEADER_FILE_STREAM_READER_2BC903C8_78B1_420B_A0B8_30FFF66CC371
