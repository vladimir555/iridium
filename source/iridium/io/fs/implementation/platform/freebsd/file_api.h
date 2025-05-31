/// \~english @brief FreeBSD-specific (and compatible systems) low-level file API declarations.
///     This header provides functions for basic file operations tailored for FreeBSD-like environments,
///     often wrapping standard C library functions.
/// \~russian @brief Специфичные для FreeBSD (и совместимых систем) объявления низкоуровневого файлового API.
///     Этот заголовочный файл предоставляет функции для базовых файловых операций, адаптированные
///     для FreeBSD-подобных окружений, часто являясь обертками для стандартных функций библиотеки C.
#ifndef HEADER_FILE_API_731A6D20_F27C_43F9_BF0B_26863A9C8A4D
#define HEADER_FILE_API_731A6D20_F27C_43F9_BF0B_26863A9C8A4D


#include "iridium/platform.h" // For FREEBSD_LIKE_PLATFORM


/// \~english @brief Conditional compilation block for FreeBSD-like platforms (including FreeBSD itself).
///     The declarations within this block are active when `FREEBSD_LIKE_PLATFORM` is defined.
/// \~russian @brief Блок условной компиляции для FreeBSD-подобных платформ (включая саму FreeBSD).
///     Объявления внутри этого блока активны, когда определен макрос `FREEBSD_LIKE_PLATFORM`.
#ifdef FREEBSD_LIKE_PLATFORM


#include "iridium/io/fs/file_stream.h" // For TFileStatus

#include <string.h> // For ::strerror
#include <stdio.h>  // For ::FILE, ::fwrite, ::fread, ::fflush etc.


namespace iridium {
namespace io {
namespace fs {
namespace implementation {
/// \~english @brief Namespace for platform-specific low-level file operations, tailored for FreeBSD-like systems.
/// \~russian @brief Пространство имен для платформо-специфичных низкоуровневых файловых операций, адаптированных для FreeBSD-подобных систем.
namespace platform {


/// \~english @brief Alias for the standard C library `fwrite` function.
///     Used for writing data to a `FILE` stream.
/// \~russian @brief Псевдоним для стандартной функции библиотеки C `fwrite`.
///     Используется для записи данных в поток `FILE`.
auto const fwriteInternal     = ::fwrite;
/// \~english @brief Alias for the standard C library `fread` function.
///     Used for reading data from a `FILE` stream.
/// \~russian @brief Псевдоним для стандартной функции библиотеки C `fread`.
///     Используется для чтения данных из потока `FILE`.
auto const freadInternal      = ::fread;
/// \~english @brief Alias for the standard C library `fflush` function.
///     Used for flushing buffered data of a `FILE` stream.
/// \~russian @brief Псевдоним для стандартной функции библиотеки C `fflush`.
///     Используется для сброса буферизованных данных потока `FILE`.
auto const fflushInternal     = ::fflush;
/// \~english @brief Alias for the standard C library `strerror` function.
///     Used for getting a human-readable string for an error number (`errno`).
/// \~russian @brief Псевдоним для стандартной функции библиотеки C `strerror`.
///     Используется для получения человекочитаемой строки для номера ошибки (`errno`).
auto const strerrorInternal   = ::strerror;

/// \~english @brief Retrieves the integer file descriptor associated with a C standard library `FILE` stream.
///     Typically wraps the `fileno()` function.
/// \~russian @brief Получает целочисленный файловый дескриптор, связанный с потоком `FILE` стандартной библиотеки C.
///     Обычно является оберткой для функции `fileno()`.
/// \~english @param file Pointer to the `FILE` stream. Must not be null.
/// \~russian @param file Указатель на поток `FILE`. Не должен быть null.
/// \~english @return The integer file descriptor.
/// \~english @throws iridium::Exception or similar if `file` is null or `fileno` fails.
/// \~russian @return Целочисленный файловый дескриптор.
/// \~russian @throws iridium::Exception или подобное исключение, если `file` равен null или `fileno` завершается ошибкой.
int getFD(::FILE *file);

/// \~english @brief Retrieves status information (like size and modification time) for an opened file.
///     Typically wraps `fstat()` by first getting the file descriptor using `getFD()`.
/// \~russian @brief Получает информацию о состоянии (например, размер и время изменения) для открытого файла.
///     Обычно является оберткой для `fstat()`, предварительно получая файловый дескриптор с помощью `getFD()`.
/// \~english @param file Pointer to an open `FILE` stream. Must not be null.
/// \~russian @param file Указатель на открытый поток `FILE`. Не должен быть null.
/// \~english @return A `TFileStatus` struct containing the file's size and last modification time.
/// \~russian @return Структура `TFileStatus`, содержащая размер файла и время последнего изменения.
/// \~english @throws iridium::Exception or similar if `file` is null or if the underlying `fstat` call fails.
/// \~russian @throws iridium::Exception или подобное исключение, если `file` равен null или если вызов `fstat` завершается ошибкой.
iridium::io::fs::TFileStatus getFileStatus(::FILE *file);

/// \~english @brief Opens a file with the specified name and mode.
///     Essentially a wrapper around the standard C library `fopen()` function.
/// \~russian @brief Открывает файл с указанным именем и режимом.
///     По сути, является оберткой для стандартной функции библиотеки C `fopen()`.
/// \~english @param file_name The path to the file to be opened.
/// \~russian @param file_name Путь к файлу, который необходимо открыть.
/// \~english @param open_mode A string specifying the file access mode (e.g., "r", "w", "a", "rb", "wb", "ab", etc.).
/// \~russian @param open_mode Строка, указывающая режим доступа к файлу (например, "r", "w", "a", "rb", "wb", "ab" и т.д.).
/// \~english @return A pointer to a `FILE` object representing the opened stream.
///     Returns `nullptr` if the file cannot be opened.
/// \~russian @return Указатель на объект `FILE`, представляющий открытый поток.
///     Возвращает `nullptr`, если файл не может быть открыт.
::FILE *open(std::string const &file_name, std::string const &open_mode);

/// \~english @brief Closes an opened file stream.
///     Essentially a wrapper around the standard C library `fclose()` function.
/// \~russian @brief Закрывает открытый файловый поток.
///     По сути, является оберткой для стандартной функции библиотеки C `fclose()`.
/// \~english @param file Pointer to the `FILE` stream to be closed. If `nullptr`, the function may do nothing.
/// \~russian @param file Указатель на поток `FILE`, который необходимо закрыть. Если `nullptr`, функция может ничего не делать.
/// \~english @throws iridium::Exception or similar if `fclose` reports an error.
/// \~russian @throws iridium::Exception или подобное исключение, если `fclose` сообщает об ошибке.
void close(::FILE *file);


} // platform
} // implementation
} // fs
} // io
} // iridium


#endif // FREEBSD_LIKE_PLATFORM


#endif // HEADER_FILE_API_731A6D20_F27C_43F9_BF0B_26863A9C8A4D
