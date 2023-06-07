// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FILE_STREAM_READER_2BC903C8_78B1_420B_A0B8_30FFF66CC371
#define HEADER_FILE_STREAM_READER_2BC903C8_78B1_420B_A0B8_30FFF66CC371


#include "iridium/io/fs/file_stream.h"
#include "file_stream.h"


namespace iridium {
namespace io {
namespace fs {
namespace implementation {


class CFileStreamReader: public CFileStream, public IFileStreamReader {
public:
    DEFINE_IMPLEMENTATION(CFileStreamReader)
    CFileStreamReader(std::string const &path);
    
    void    initialize()  override;
    void    finalize()    override;

    TID     getID() const override;
    URI::TSharedPtr getURI() const override;

    Buffer::TSharedPtr read(size_t const &size) override;
    size_t  getSize() override;
    TFileStatus getStatus() override;
};


} // implementation
} // fs
} // io
} // iridium


#endif // HEADER_FILE_STREAM_READER_2BC903C8_78B1_420B_A0B8_30FFF66CC371
