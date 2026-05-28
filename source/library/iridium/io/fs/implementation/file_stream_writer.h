// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FILE_STREAM_WRITER_06F9B4BF_E655_4FB7_8EA0_185306B7C2A3
#define HEADER_FILE_STREAM_WRITER_06F9B4BF_E655_4FB7_8EA0_185306B7C2A3


#include "file_stream.h"


namespace iridium::io::fs::implementation {


//! \~english @deprecated todo: rm, deprecated
//! \~russian @deprecated todo: rm, deprecated
class CFileStreamWriter: public virtual CFileStream {
public:
    DEFINE_IMPLEMENTATION(CFileStreamWriter)
    CFileStreamWriter(std::string const &path, bool const &is_rewrite = false);
};


} // iridium::io::fs::implementation


#endif // HEADER_FILE_STREAM_WRITER_06F9B4BF_E655_4FB7_8EA0_185306B7C2A3
