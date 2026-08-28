// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "file_stream_reader.h"

#include "iridium/convertion/convert.h"


using iridium::convertion::convert;


namespace iridium::io::fs::implementation {


CFileStreamReader::CFileStreamReader(std::string const &path)
:
    CFileStream(path, TOpenMode::READ)
{}


} // iridium::io::fs::implementation
