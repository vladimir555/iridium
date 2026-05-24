// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines the `CFileCache` class, a file-backed implementation of the `ICache` interface.
/// \~russian @brief Определяет класс `CFileCache`, файловую реализацию интерфейса `ICache`.
#ifndef HEADER_FILE_CACHE_8ABCB65B_D74A_48BA_80C2_E6B63B28786F
#define HEADER_FILE_CACHE_8ABCB65B_D74A_48BA_80C2_E6B63B28786F


#include <string>
#include <vector>
#include <memory> // For std::shared_ptr

#include "iridium/smart_ptr.h"           // For DEFINE_CREATE, TSharedPtr
#include "iridium/io/fs/cache.h"         // For ICache interface
#include "iridium/io/fs/file_stream.h"   // For IFileStreamWriter


namespace iridium::io::fs::implementation {


/// \~english @brief A file-backed cache implementation for strings.
///     This class implements the `ICache` interface, providing a mechanism to store strings
///     in memory with the ability to load from and persist (flush) to a specified file.
///     Strings are typically accessed by an ID, which in this implementation corresponds
///     to their index in an internal vector.
/// \~russian @brief Реализация кэша строк с файловым хранилищем.
///     Этот класс реализует интерфейс `ICache`, предоставляя механизм для хранения строк
///     в памяти с возможностью загрузки из файла и сохранения (сброса) в указанный файл.
///     Доступ к строкам обычно осуществляется по идентификатору, который в этой реализации соответствует
///     их индексу во внутреннем векторе.
class CFileCache : public ICache {
public:
    /// \~english @brief Defines smart pointer typedefs (e.g., `CFileCache::TSharedPtr`) for this class.
    /// \~russian @brief Определяет typedef'ы умных указателей (например, `CFileCache::TSharedPtr`) для этого класса.
    DEFINE_CREATE(CFileCache)
    explicit CFileCache(std::string const &file_name);
    virtual ~CFileCache() = default;

    virtual void initialize() override;
    virtual void finalize() override;
    virtual size_t push(std::string const &line) override;
    virtual std::string get(size_t const &id) const override;
    virtual void remove(size_t const &id) override;
    virtual void clean() override;
    virtual size_t size() const override;
    virtual bool checkExistense(size_t const &id) const override;
    virtual void flush() override;

private:
    std::string
        m_file_name;
    IFileStreamWriter::TSharedPtr
        m_file_writer;
    std::vector< std::shared_ptr<std::string> >
        m_lines;
};


} // iridium::io::fs::implementation


#endif // HEADER_FILE_CACHE_8ABCB65B_D74A_48BA_80C2_E6B63B28786F
