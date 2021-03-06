// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FILE_CACHE_8ABCB65B_D74A_48BA_80C2_E6B63B28786F
#define HEADER_FILE_CACHE_8ABCB65B_D74A_48BA_80C2_E6B63B28786F


#include <string>
#include <vector>

#include "iridium/smart_ptr.h"
#include "iridium/io/fs/cache.h"
#include "iridium/io/fs/file_stream.h"


namespace iridium {
namespace io {
namespace fs {
namespace implementation {


class CFileCache : public ICache {
public:
    DEFINE_CREATE(CFileCache)
    ///
    explicit CFileCache(std::string const &file_name);
    ///
    virtual ~CFileCache() = default;
    ///
    virtual void initialize() override;
    ///
    virtual void finalize() override;
    ///
    virtual size_t push(std::string const &line) override;
    ///
    virtual std::string get(size_t const &id) const override;
    ///
    virtual void remove(size_t const &id) override;
    ///
    virtual void clean() override;
    ///
    virtual size_t size() const override;
    ///
    virtual bool checkExistense(size_t const &id) const override;
    ///
    virtual void flush() override;

private:
    ///
    std::string                     m_file_name;
    ///
    IFileStreamWriter::TSharedPtr   m_file_writer;
    ///
    std::vector< std::shared_ptr<std::string> > m_lines;
};


} // implementation
} // fs
} // io
} // iridium


#endif // HEADER_FILE_CACHE_8ABCB65B_D74A_48BA_80C2_E6B63B28786F
