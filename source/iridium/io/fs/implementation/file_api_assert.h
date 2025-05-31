/// \~english @brief Defines assertion helper functions for low-level file API calls.
///     These functions wrap results from file operations and throw exceptions if errors are detected.
/// \~russian @brief Определяет вспомогательные функции проверки утверждений для низкоуровневых вызовов файлового API.
///     Эти функции оборачивают результаты файловых операций и выбрасывают исключения при обнаружении ошибок.
#ifndef HEADER_FILE_API_ASSERT_B07BE67D_925C_4201_92DB_A20EBE710DE1
#define HEADER_FILE_API_ASSERT_B07BE67D_925C_4201_92DB_A20EBE710DE1


#include <stdio.h>  // For FILE*
#include <string>   // For std::string
#include <stdexcept> // For std::runtime_error (likely thrown)
// Potentially: #include "iridium/Exception.h" if a custom exception is used.


namespace iridium {
namespace io {
namespace fs {
/// \~english @brief Namespace for internal implementation details of file system functionalities, including error checking utilities.
/// \~russian @brief Пространство имен для внутренних деталей реализации функциональностей файловой системы, включая утилиты проверки ошибок.
namespace implementation {


/// \~english @brief Checks an integer result code from a file operation and throws an exception on error.
///     Commonly used for C-style functions where a negative return value indicates an error.
///     If an error is detected (typically if `result` < 0), a `std::runtime_error` (or a similar Iridium-specific exception)
///     is thrown, incorporating the provided message and system error information (e.g., from `errno`).
/// \~russian @brief Проверяет целочисленный код результата файловой операции и выбрасывает исключение при ошибке.
///     Обычно используется для функций в стиле C, где отрицательное возвращаемое значение указывает на ошибку.
///     При обнаружении ошибки (обычно если `result` < 0) выбрасывается `std::runtime_error` (или аналогичное специфичное для Iridium исключение),
///     включающее предоставленное сообщение и информацию о системной ошибке (например, из `errno`).
/// \~english @param result The integer result code to check.
/// \~russian @param result Целочисленный код результата для проверки.
/// \~english @param message A custom message to include in the exception if an error occurs.
/// \~russian @param message Пользовательское сообщение для включения в исключение в случае ошибки.
/// \~english @return The original `result` if no error is detected, allowing for chained assignments or checks.
/// \~russian @return Исходное значение `result`, если ошибка не обнаружена, что позволяет использовать цепочки присваиваний или проверок.
/// \~english @throws std::runtime_error (or similar) if `result` indicates an error (e.g., < 0).
/// \~russian @throws std::runtime_error (или аналогичное), если `result` указывает на ошибку (например, < 0).
int assertOK(const int &result, const std::string &message);


/// \~english @brief Checks a `FILE*` pointer from a C standard I/O file operation and throws an exception on error.
///     Used for functions like `fopen`, where a `nullptr` return value indicates an error.
///     If `result` is `nullptr`, a `std::runtime_error` (or a similar Iridium-specific exception)
///     is thrown, incorporating the provided message and system error information (e.g., from `errno`).
/// \~russian @brief Проверяет указатель `FILE*` из стандартной операции ввода-вывода C и выбрасывает исключение при ошибке.
///     Используется для функций типа `fopen`, где возвращаемое значение `nullptr` указывает на ошибку.
///     Если `result` равен `nullptr`, выбрасывается `std::runtime_error` (или аналогичное специфичное для Iridium исключение),
///     включающее предоставленное сообщение и информацию о системной ошибке (например, из `errno`).
/// \~english @param result The `FILE*` pointer to check.
/// \~russian @param result Указатель `FILE*` для проверки.
/// \~english @param message A custom message to include in the exception if `result` is `nullptr`.
/// \~russian @param message Пользовательское сообщение для включения в исключение, если `result` равен `nullptr`.
/// \~english @return The original `result` pointer if it is not `nullptr`.
/// \~russian @return Исходный указатель `result`, если он не `nullptr`.
/// \~english @throws std::runtime_error (or similar) if `result` is `nullptr`.
/// \~russian @throws std::runtime_error (или аналогичное), если `result` равен `nullptr`.
::FILE *assertOK(::FILE *result, const std::string &message);


} // implementation
} // fs
} // io
} // iridium


#endif // HEADER_FILE_API_ASSERT_B07BE67D_925C_4201_92DB_A20EBE710DE1
