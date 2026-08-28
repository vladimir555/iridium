// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "file_stream_writer.h"


namespace iridium::io::fs::implementation {


CFileStreamWriter::CFileStreamWriter(std::string const &path, bool const &is_rewrite)
:
    CFileStream(path, is_rewrite ? TOpenMode::REWRITE : TOpenMode::WRITE)
{}


} // iridium::io::fs::implementation
