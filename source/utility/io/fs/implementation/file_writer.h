#ifndef HEADER_FILE_WRITER_6A05934B_514A_4E85_97E7_69A721D4ED29
#define HEADER_FILE_WRITER_6A05934B_514A_4E85_97E7_69A721D4ED29


#include <string>
#include <stdio.h>

#include "utility/smart_ptr.h"

#include "utility/io/fs/file_stream.h"


namespace utility {
namespace io {
namespace fs {
namespace implementation {


class CFileStream:
    public IFileWriter,
    public IFileReader
{
public:
    DEFINE_CREATE(CFileStream)
    explicit CFileStream(std::string const &file_name);
    virtual ~CFileStream();

    void initialize() override; 
    void finalize() override;

    size_t write(TBuffer const &packet) override;
    TBuffer read(size_t const &size) override;

    void flush() override;
//    int getID() const override;

private:
    std::string     m_file_name;
    ::FILE         *m_file = nullptr;
};


} // implementation
} // fs
} // io
} // utility


#endif // HEADER_FILE_WRITER_6A05934B_514A_4E85_97E7_69A721D4ED29
