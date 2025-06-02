// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONTENT_STORAGE_C92EBD57_DA63_4A76_8A56_343A9E6FEFCB
#define HEADER_CONTENT_STORAGE_C92EBD57_DA63_4A76_8A56_343A9E6FEFCB


#include "iridium/io/protocol/http/content_storage.h"


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


/// \~english @brief Concrete implementation of `IContentStorage` that serves content from a local file system directory.
/// \~russian @brief Конкретная реализация `IContentStorage`, которая обслуживает контент из локального каталога файловой системы.
class CContentStorage: public IContentStorage {
public:
    DEFINE_IMPLEMENTATION(CContentStorage)
    /// \~english @brief Constructs a `CContentStorage` instance.
    /// \~russian @brief Конструирует экземпляр `CContentStorage`.
    /// \~english @param root_path The root directory path from which content will be served.
    /// \~russian @param root_path Корневой путь к каталогу, из которого будет обслуживаться контент.
    CContentStorage(std::string const &root_path);

    /// \~english @brief Retrieves content as a readable file stream from the configured root path.
    ///     The requested `path` is appended to the `m_root_path` to locate the file.
    /// \~russian @brief Получает контент в виде читаемого файлового потока из настроенного корневого пути.
    ///     Запрошенный `path` добавляется к `m_root_path` для определения местоположения файла.
    /// \~english @param path The relative path to the content within the root directory.
    /// \~russian @param path Относительный путь к контенту в корневом каталоге.
    /// \~english @return A shared pointer to an `IFileStreamReader` for the requested file.
    /// \~english Returns `nullptr` if the file does not exist or cannot be opened (e.g., due to permissions).
    /// \~russian @return Умный указатель на `IFileStreamReader` для запрошенного файла.
    /// \~russian Возвращает `nullptr`, если файл не существует или не может быть открыт (например, из-за прав доступа).
    fs::IFileStreamReader::TSharedPtr getContent(std::string const &path) override;
    
private:
    /// \~english @brief The root directory path from which files are served.
    /// \~russian @brief Корневой путь к каталогу, из которого обслуживаются файлы.
    std::string m_root_path;
};


} // implementation
} // http
} // protocol
} // io
} // iridium


#endif // HEADER_CONTENT_STORAGE_C92EBD57_DA63_4A76_8A56_343A9E6FEFCB
