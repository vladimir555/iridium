// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONTENT_STORAGE_357A6B8F_B845_4480_B73F_8334B28B71A6
#define HEADER_CONTENT_STORAGE_357A6B8F_B845_4480_B73F_8334B28B71A6


#include "iridium/smart_ptr.h"
#include "iridium/io/fs/file_stream.h"
#include "iridium/pattern/initializable.h"
#include <string>


namespace iridium {
namespace io {
namespace protocol {
namespace http {


/// \~english @brief Interface for a content storage system, used to retrieve HTTP content (e.g., files) based on a path.
/// \~russian @brief Интерфейс для системы хранения контента, используемый для получения HTTP-контента (например, файлов) по указанному пути.
class IContentStorage {
public:
    DEFINE_INTERFACE(IContentStorage)
    /// \~english @brief Retrieves content as a readable file stream from the storage.
    /// \~russian @brief Получает контент в виде читаемого файлового потока из хранилища.
    /// \~english @param path The path to the content (e.g., a relative file path for a web server).
    /// \~russian @param path Путь к контенту (например, относительный путь к файлу для веб-сервера).
    /// \~english @return A shared pointer to an `IFileStreamReader` for accessing the content.
    /// \~english Returns `nullptr` or throws an exception if the content is not found or cannot be accessed.
    /// \~russian @return Умный указатель на `IFileStreamReader` для доступа к контенту.
    /// \~russian Возвращает `nullptr` или выбрасывает исключение, если контент не найден или к нему нет доступа.
    virtual fs::IFileStreamReader::TSharedPtr getContent(std::string const &path) = 0;
};


} // http
} // protocol
} // io
} // iridium


#endif // HEADER_CONTENT_STORAGE_357A6B8F_B845_4480_B73F_8334B28B71A6
