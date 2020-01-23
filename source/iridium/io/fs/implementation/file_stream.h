// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FILE_STREAM_6A05934B_514A_4E85_97E7_69A721D4ED29
#define HEADER_FILE_STREAM_6A05934B_514A_4E85_97E7_69A721D4ED29


#include <string>
#include <stdio.h>

#include "iridium/smart_ptr.h"
#include "iridium/enum.h"
#include "iridium/io/stream.h"
#include "iridium/io/fs/file_stream.h"


namespace iridium {
namespace io {
namespace fs {
namespace implementation {


class CFileStream: public IStream {
public:
    DEFINE_ENUM(TOpenMode, READ, WRITE)

    void    initialize() override;
    void    finalize() override;

    int     getID() const override;
    
    size_t  getSize() const;
    
    Buffer::TSharedPtr read (size_t const &size);
    size_t  write(Buffer const &buffer);

    void    flush();

    TFileStatus getStatus();

protected:
    DEFINE_CREATE(CFileStream)

    explicit CFileStream(std::string const &file_name, TOpenMode const &open_mode);
    virtual ~CFileStream() override;

private:
    std::string     m_file_name;
    ::FILE         *m_file = nullptr;
    TOpenMode       m_open_mode;
};


} // implementation
} // fs
} // io
} // iridium


#endif // HEADER_FILE_STREAM_6A05934B_514A_4E85_97E7_69A721D4ED29
