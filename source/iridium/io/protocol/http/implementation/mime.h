// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_MIME_B406ABE9_2B4C_4F52_A8E9_CA92EA867A09
#define HEADER_MIME_B406ABE9_2B4C_4F52_A8E9_CA92EA867A09


#include <list>
#include <string>

#include "iridium/pattern/singleton.h"


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


/// \~english @brief Singleton class for managing and looking up MIME types based on file extensions.
///     It preloads a list of common MIME types and their associated extensions.
///     The class is designed to be read-only after initialization, hence "not synchronized".
/// \~russian @brief Класс-одиночка для управления и поиска MIME-типов на основе расширений файлов.
///     Он предварительно загружает список распространенных MIME-типов и связанных с ними расширений.
///     Класс предназначен только для чтения после инициализации, поэтому он "не синхронизирован".
class MIME: public pattern::Singleton<MIME> {
public:
    /// \~english @brief Virtual destructor.
    /// \~russian @brief Виртуальный деструктор.
    virtual ~MIME() = default;

    /// \~english @brief Gets the MIME type string for a given file name extension.
    /// \~russian @brief Получает строку MIME-типа для заданного расширения файла.
    /// \~english @param extension The file extension (e.g., "txt", "html", "jpg") without the leading dot. Case-insensitive.
    /// \~russian @param extension Расширение файла (например, "txt", "html", "jpg") без ведущей точки. Регистронезависимый.
    /// \~english @return The corresponding MIME type string (e.g., "text/plain", "image/jpeg").
    /// \~english Returns "application/octet-stream" if the extension is not found.
    /// \~russian @return Строка соответствующего MIME-типа (например, "text/plain", "image/jpeg").
    /// \~russian Возвращает "application/octet-stream", если расширение не найдено.
    std::string getByFileNameExtension(std::string const &extension) const;

private:
    /// \~english @brief Private constructor to enforce singleton pattern.
    ///     Initializes the list of known MIME types and their extensions.
    /// \~russian @brief Приватный конструктор для реализации шаблона "одиночка".
    ///     Инициализирует список известных MIME-типов и их расширений.
    MIME();
    friend class pattern::Singleton<MIME>;

    /// \~english @brief Structure to hold a MIME type and its associated file extensions.
    /// \~russian @brief Структура для хранения MIME-типа и связанных с ним расширений файлов.
    struct TMIME {
        /// \~english @brief A list of file extensions (without leading dot) that map to this MIME type.
        /// \~russian @brief Список расширений файлов (без ведущей точки), которые соответствуют этому MIME-типу.
        std::list<std::string>  extensions;
        /// \~english @brief The MIME type string (e.g., "text/html").
        /// \~russian @brief Строка MIME-типа (например, "text/html").
        std::string             type;
    };

    /// \~english @brief A list of `TMIME` structures representing the known MIME type mappings.
    /// \~russian @brief Список структур `TMIME`, представляющих известные сопоставления MIME-типов.
    std::list<TMIME> m_mimes;
};


} // implementation
} // http
} // protocol
} // io
} // iridium


#endif // HEADER_MIME_B406ABE9_2B4C_4F52_A8E9_CA92EA867A09
