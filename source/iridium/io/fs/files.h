// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Declares utility functions for file system operations.
/// \~russian @brief Объявляет служебные функции для операций с файловой системой.
#ifndef HEADER_FS_EC4EF5DB_F63C_4227_8830_C306F3060E07
#define HEADER_FS_EC4EF5DB_F63C_4227_8830_C306F3060E07


#include <string>
#include <vector>


namespace iridium {
namespace io {
/// \~english @brief Namespace for file system related I/O functionalities, including file operations.
/// \~russian @brief Пространство имен для функциональности ввода-вывода, связанной с файловой системой, включая операции с файлами.
namespace fs {


/// \~english @brief Reads all lines from a text file into a vector of strings.
///     Each element in the returned vector represents a line from the file.
/// \~russian @brief Читает все строки из текстового файла в вектор строк.
///     Каждый элемент в возвращаемом векторе представляет строку из файла.
/// \~english @param file_name The path to the text file to be read.
/// \~russian @param file_name Путь к текстовому файлу для чтения.
/// \~english @param is_throw_if_not_exists If `true` (default), throws an exception if the file does not exist.
///     If `false` and the file does not exist, an empty vector is returned.
/// \~russian @param is_throw_if_not_exists Если `true` (по умолчанию), выбрасывает исключение, если файл не существует.
///     Если `false` и файл не существует, возвращается пустой вектор.
/// \~english @return A `std::vector<std::string>` containing the lines of the file.
/// \~russian @return `std::vector<std::string>`, содержащий строки файла.
/// \~english @throws std::runtime_error or similar if `is_throw_if_not_exists` is true and the file doesn't exist, or if other I/O errors occur.
/// \~russian @throws std::runtime_error или подобное исключение, если `is_throw_if_not_exists` истинно и файл не существует, или если возникают другие ошибки ввода-вывода.
std::vector<std::string> readTextFile(std::string const &file_name, bool const &is_throw_if_not_exists = true);


/// \~english @brief Reads the entire content of a file into a single string.
/// \~russian @brief Читает все содержимое файла в одну строку.
/// \~english @param file_name The path to the file to be read.
/// \~russian @param file_name Путь к файлу для чтения.
/// \~english @return A `std::string` containing the entire content of the file.
///     Returns an empty string if the file is empty or cannot be read (behavior on error might vary, e.g., throw).
/// \~russian @return `std::string`, содержащая все содержимое файла.
///     Возвращает пустую строку, если файл пуст или не может быть прочитан (поведение при ошибке может варьироваться, например, выброс исключения).
/// \~english @throws std::runtime_error or similar if the file cannot be opened or read.
/// \~russian @throws std::runtime_error или подобное исключение, если файл не может быть открыт или прочитан.
std::string readFile(std::string const &file_name);


/// \~english @brief Writes a given string to a file, overwriting the file if it already exists.
///     If the file does not exist, it is created.
/// \~russian @brief Записывает данную строку в файл, перезаписывая файл, если он уже существует.
///     Если файл не существует, он создается.
/// \~english @param file_name The path to the file where the text should be written.
/// \~russian @param file_name Путь к файлу, в который следует записать текст.
/// \~english @param text The `std::string` content to write to the file.
/// \~russian @param text Содержимое `std::string` для записи в файл.
/// \~english @throws std::runtime_error or similar if the file cannot be opened for writing or if a write error occurs.
/// \~russian @throws std::runtime_error или подобное исключение, если файл не может быть открыт для записи или если возникает ошибка записи.
void writeFile(std::string const &file_name, std::string const &text);


/// \~english @brief Checks if a file exists at the specified path.
/// \~russian @brief Проверяет, существует ли файл по указанному пути.
/// \~english @param file_name The path to the file to check.
/// \~russian @param file_name Путь к файлу для проверки.
/// \~english @return `true` if a file exists at the given path, `false` otherwise.
/// \~russian @return `true`, если файл существует по данному пути, иначе `false`.
bool checkFileExistence(std::string const &file_name);


/// \~english @brief Removes (deletes) the specified file.
/// \~russian @brief Удаляет указанный файл.
/// \~english @param file_name The path to the file to be removed.
/// \~russian @param file_name Путь к удаляемому файлу.
/// \~english @throws std::runtime_error or similar if the file cannot be removed (e.g., does not exist, permissions error).
/// \~russian @throws std::runtime_error или подобное исключение, если файл не может быть удален (например, не существует, ошибка прав доступа).
void remove(std::string const &file_name);


/// \~english @brief Extracts the extension from a filename.
///     The extension is considered to be the part of the filename after the last dot (`.`).
///     If no dot is found, or if the dot is the first character, an empty string may be returned.
/// \~russian @brief Извлекает расширение из имени файла.
///     Расширением считается часть имени файла после последней точки (`.`).
///     Если точка не найдена или является первым символом, может быть возвращена пустая строка.
/// \~english @param file_name The filename from which to extract the extension.
/// \~russian @param file_name Имя файла, из которого извлекается расширение.
/// \~english @return A `std::string` containing the file extension (without the dot), or an empty string if no extension is found.
/// \~russian @return `std::string`, содержащая расширение файла (без точки), или пустая строка, если расширение не найдено.
std::string extractFileNameExtension(std::string const &file_name);


} // fs
} // io
} // iridium


#endif // HEADER_FS_EC4EF5DB_F63C_4227_8830_C306F3060E07
