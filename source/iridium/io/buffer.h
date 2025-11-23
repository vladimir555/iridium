// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
#define HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1


/// \~english @file
/// @brief Defines the `Buffer` class for handling raw binary data.
/// \~russian @file
/// @brief Определяет класс `Buffer` для работы с сырыми двоичными данными.


#include "iridium/convertion/convert.h"
#include "iridium/smart_ptr.h"

#include <vector>
#include <string>
#include <list>


namespace iridium::io {


/// \~english @brief A Buffer class for handling raw binary data, inheriting from std::vector<uint8_t>.
/// \~english @details Provides various constructors to create buffers from strings or other buffers, and utility functions for data manipulation, such as checking suffixes.
/// \~russian @brief Класс Buffer для работы с сырыми двоичными данными, унаследованный от std::vector<uint8_t>.
/// \~russian @details Предоставляет различные конструкторы для создания буферов из строк или других буферов, а также вспомогательные функции для манипулирования данными, например, для проверки суффиксов.
class Buffer: public std::vector<uint8_t> {
public:
    DEFINE_CREATE(Buffer)

    /// \~english @brief Default constructor.
    /// \~russian @brief Конструктор по умолчанию.
    Buffer() = default;

    /// \~english @brief Constructs a buffer from a C-style string.
    /// \~russian @brief Конструирует буфер из строки в стиле C.
    Buffer(char const * const str);

    /// \~english @brief Constructs a buffer from a `std::string`.
    /// \~russian @brief Конструирует буфер из `std::string`.
    Buffer(std::string const &str);

    /// \~english @brief Constructs a buffer from a list of shared pointers to other buffers.
    /// \~russian @brief Конструирует буфер из списка умных указателей на другие буферы.
    Buffer(std::list<Buffer::TSharedPtr> const &buffers);

    /// \~english @brief Constructs a buffer from a variadic list of arguments.
    /// \~russian @brief Конструирует буфер из вариативного списка аргументов.
    template<typename ... TArgs>
    Buffer(TArgs ... args);

    /// \~english @brief Checks if the buffer ends with a given suffix, optionally skipping some characters.
    /// \~russian @brief Проверяет, заканчивается ли буфер заданным суффиксом, опционально пропуская некоторые символы.
    bool checkSuffixEqual(std::string const &suffix, std::string const &skip = "") const;

    /// \~english @brief Checks if the buffer ends with a given suffix.
    /// \~russian @brief Проверяет, заканчивается ли буфер заданным суффиксом.
    bool checkSuffixEqual(uint8_t const * const suffix, size_t const &size) const;

    /// \~english @brief Checks if the buffer ends with a given suffix.
    /// \~russian @brief Проверяет, заканчивается ли буфер заданным суффиксом.
    bool checkSuffixEqual(std::vector<uint8_t> const &suffix) const;

    /// \~english @brief Appends another buffer to the end of this one.
    /// \~russian @brief Добавляет другой буфер в конец этого.
    void emplace_back(TSharedPtr const &);
};


template<typename ... TArgs>
Buffer::Buffer(TArgs ... args)
:
    std::vector<uint8_t>(args ...)
{}


} // namespace iridium::io


DEFINE_CONVERT(iridium::io::Buffer, std::string)
DEFINE_CONVERT(std::string, iridium::io::Buffer)


#endif // HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
