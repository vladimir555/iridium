// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FILE_STREAM_READER_2BC903C8_78B1_420B_A0B8_30FFF66CC371
#define HEADER_FILE_STREAM_READER_2BC903C8_78B1_420B_A0B8_30FFF66CC371


#include "file_stream.h"


namespace iridium {
namespace io {
namespace fs {
namespace implementation {


class CFileStreamReader: public virtual CFileStream {
public:
    DEFINE_IMPLEMENTATION(CFileStreamReader)
    CFileStreamReader(std::string const &path);
};


} // implementation
} // fs
} // io
} // iridium


#endif // HEADER_FILE_STREAM_READER_2BC903C8_78B1_420B_A0B8_30FFF66CC371
