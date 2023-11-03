// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FILE_STREAM_6A05934B_514A_4E85_97E7_69A721D4ED29
#define HEADER_FILE_STREAM_6A05934B_514A_4E85_97E7_69A721D4ED29


#include "iridium/enum.h"
#include "iridium/io/fs/file_stream.h"
#include "iridium/pattern/non_copyable.h"

#include <string>
#include <stdio.h>


namespace iridium {
namespace io {
namespace fs {
namespace implementation {


class CFileStream:
    public virtual pattern::NonCopyable,
    public virtual IFileStreamReader,
    public virtual IFileStreamWriter
{
public:
    DEFINE_ENUM(TOpenMode, READ, WRITE, REWRITE)

    virtual ~CFileStream();

    void initialize()   override;
    void finalize()     override;

    std::list<uintptr_t>    getHandles()    const override;
    URI::TSharedPtr         getURI()        const override;
    TFileStatus             getStatus()     const override;

    Buffer::TSharedPtr      read(size_t const &size) override;
    size_t                  write(Buffer::TSharedPtr const &buffer) override;
    void                    flush() override;

protected:

    explicit CFileStream(std::string const &file_name, TOpenMode const &open_mode);

private:
    std::string     m_file_name;
    ::FILE         *m_file = nullptr;
    TOpenMode       m_open_mode;
    URI::TSharedPtr m_uri;
};


} // implementation
} // fs
} // io
} // iridium


#endif // HEADER_FILE_STREAM_6A05934B_514A_4E85_97E7_69A721D4ED29
