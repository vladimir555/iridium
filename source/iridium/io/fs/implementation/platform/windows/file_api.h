/// \~english @brief Windows-platform specific low-level file API declarations.
///     This header provides functions for basic file operations tailored for the Windows environment,
///     often wrapping secure CRT functions or standard C library functions as implemented on Windows.
/// \~russian @brief Специфичные для платформы Windows объявления низкоуровневого файлового API.
///     Этот заголовочный файл предоставляет функции для базовых файловых операций, адаптированные
///     для окружения Windows, часто являясь обертками для безопасных функций CRT или стандартных функций библиотеки C,
///     реализованных в Windows.
#ifndef HEADER_FILE_API_6C03CE66_6672_4286_B6F3_C6E87D440ABF
#define HEADER_FILE_API_6C03CE66_6672_4286_B6F3_C6E87D440ABF


#include "iridium/platform.h" // For WINDOWS_PLATFORM


/// \~english @brief Conditional compilation block for the Windows platform.
///     The declarations within this block are active when `WINDOWS_PLATFORM` is defined.
/// \~russian @brief Блок условной компиляции для платформы Windows.
///     Объявления внутри этого блока активны, когда определен макрос `WINDOWS_PLATFORM`.
#ifdef WINDOWS_PLATFORM


#include "iridium/io/fs/file_stream.h" // For TFileStatus

#include <string.h> // For standard string functions, possibly strerror if not custom
#include <stdio.h>  // For ::FILE, ::fwrite, ::fread, ::fflush etc.


namespace iridium {
namespace io {
namespace fs {
namespace implementation {
/// \~english @brief Namespace for platform-specific low-level file operations, tailored for Windows.
/// \~russian @brief Пространство имен для платформо-специфичных низкоуровневых файловых операций, адаптированных для Windows.
namespace platform {


/// \~english @brief Alias for the standard C library `fwrite` function (Windows CRT version).
///     Used for writing data to a `FILE` stream.
/// \~russian @brief Псевдоним для стандартной функции библиотеки C `fwrite` (версия Windows CRT).
///     Используется для записи данных в поток `FILE`.
auto const fwriteInternal     = ::fwrite;
/// \~english @brief Alias for the standard C library `fread` function (Windows CRT version).
///     Used for reading data from a `FILE` stream.
/// \~russian @brief Псевдоним для стандартной функции библиотеки C `fread` (версия Windows CRT).
///     Используется для чтения данных из потока `FILE`.
auto const freadInternal      = ::fread;
/// \~english @brief Alias for the standard C library `fflush` function (Windows CRT version).
///     Used for flushing buffered data of a `FILE` stream.
/// \~russian @brief Псевдоним для стандартной функции библиотеки C `fflush` (версия Windows CRT).
///     Используется для сброса буферизованных данных потока `FILE`.
auto const fflushInternal     = ::fflush;

/// \~english @brief Retrieves a human-readable string for a given system error code on Windows.
///     This function likely wraps Windows-specific error reporting like `FormatMessage` or `strerror_s`
///     for better error messages or thread safety compared to standard `strerror`.
/// \~russian @brief Получает человекочитаемую строку для заданного кода системной ошибки в Windows.
///     Эта функция, вероятно, оборачивает специфичные для Windows функции сообщения об ошибках, такие как `FormatMessage` или `strerror_s`,
///     для улучшения сообщений об ошибках или потокобезопасности по сравнению со стандартной `strerror`.
/// \~english @param code The error code (e.g., from `errno` or `GetLastError()`).
/// \~russian @param code Код ошибки (например, из `errno` или `GetLastError()`).
/// \~english @return A `std::string` describing the error.
/// \~russian @return `std::string`, описывающая ошибку.
std::string strerrorInternal(int const &code);

/// \~english @brief Retrieves the integer file descriptor associated with a C standard library `FILE` stream on Windows.
///     Typically wraps the `_fileno()` function.
/// \~russian @brief Получает целочисленный файловый дескриптор, связанный с потоком `FILE` стандартной библиотеки C в Windows.
///     Обычно является оберткой для функции `_fileno()`.
/// \~english @param file Pointer to the `FILE` stream. Must not be null.
/// \~russian @param file Указатель на поток `FILE`. Не должен быть null.
/// \~english @return The integer file descriptor.
/// \~english @throws iridium::Exception or similar if `file` is null or `_fileno` fails.
/// \~russian @return Целочисленный файловый дескриптор.
/// \~russian @throws iridium::Exception или подобное исключение, если `file` равен null или `_fileno` завершается ошибкой.
int getFD(::FILE *file);

/// \~english @brief Retrieves status information (like size and modification time) for an opened file on Windows.
///     Typically wraps `_fstat64` (or similar) by first getting the file descriptor using `getFD()`.
/// \~russian @brief Получает информацию о состоянии (например, размер и время изменения) для открытого файла в Windows.
///     Обычно является оберткой для `_fstat64` (или подобной функции), предварительно получая файловый дескриптор с помощью `getFD()`.
/// \~english @param file Pointer to an open `FILE` stream. Must not be null.
/// \~russian @param file Указатель на открытый поток `FILE`. Не должен быть null.
/// \~english @return A `TFileStatus` struct containing the file's size and last modification time.
/// \~russian @return Структура `TFileStatus`, содержащая размер файла и время последнего изменения.
/// \~english @throws iridium::Exception or similar if `file` is null or if the underlying stat call fails.
/// \~russian @throws iridium::Exception или подобное исключение, если `file` равен null или если вызов функции stat завершается ошибкой.
iridium::io::fs::TFileStatus getFileStatus(::FILE *file);

/// \~english @brief Opens a file with the specified name and mode on Windows.
///     Likely a wrapper around the secure `fopen_s()` function or standard `fopen()`.
/// \~russian @brief Открывает файл с указанным именем и режимом в Windows.
///     Вероятно, является оберткой для безопасной функции `fopen_s()` или стандартной `fopen()`.
/// \~english @param file_name The path to the file to be opened.
/// \~russian @param file_name Путь к файлу, который необходимо открыть.
/// \~english @param open_mode A string specifying the file access mode (e.g., "r", "w", "a", "rb", "wb", "ab", etc.).
/// \~russian @param open_mode Строка, указывающая режим доступа к файлу (например, "r", "w", "a", "rb", "wb", "ab" и т.д.).
/// \~english @return A pointer to a `FILE` object representing the opened stream.
///     Returns `nullptr` if the file cannot be opened.
/// \~russian @return Указатель на объект `FILE`, представляющий открытый поток.
///     Возвращает `nullptr`, если файл не может быть открыт.
::FILE *open(std::string const &file_name, std::string const &open_mode);

/// \~english @brief Closes an opened file stream on Windows.
///     Essentially a wrapper around the standard C library `fclose()` function.
/// \~russian @brief Закрывает открытый файловый поток в Windows.
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


#endif // WINDOWS_PLATFORM


#endif // HEADER_FILE_API_6C03CE66_6672_4286_B6F3_C6E87D440ABF
