#ifndef HEADER_FAST_TEXT_WRITER_6A05934B_514A_4E85_97E7_69A721D4ED29
#define HEADER_FAST_TEXT_WRITER_6A05934B_514A_4E85_97E7_69A721D4ED29


#include <string>
#include <stdio.h>

#include "utility/smart_ptr.h"

#include "utility/fs/text_writer.h"


namespace utility {
namespace fs {
namespace implementation {


class CFileWriter : public IFileWriter {
public:
    DEFINE_CREATE(CFileWriter)
    ///
    explicit CFileWriter(std::string const &file_name);
    ///
    virtual ~CFileWriter();
    ///
    void initialize() override; 
    ///
    void finalize() override;
    ///
//    size_t writeLine(std::string const &line) override;
    size_t write(io::TBuffer const &packet) override;
    ///
    void flush() override;

private:
    ///
    std::string     m_file_name;
    ///
    ::FILE         *m_file = nullptr;
};


} // implementation
} // fs
} // utility


#endif // HEADER_FAST_TEXT_WRITER_6A05934B_514A_4E85_97E7_69A721D4ED29
