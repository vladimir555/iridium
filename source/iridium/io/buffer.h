// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
#define HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1


#include "iridium/convertion/convert.h"
#include "iridium/smart_ptr.h"

#include <vector>
#include <string>
#include <list>


namespace iridium {
namespace io {


/// \~english @brief A Buffer class for handling raw binary data, inheriting from std::vector<uint8_t>.
/// \~english @details Provides various constructors to create buffers from strings or other buffers, and utility functions for data manipulation, such as checking suffixes.
/// \~russian @brief Класс Buffer для работы с сырыми двоичными данными, унаследованный от std::vector<uint8_t>.
/// \~russian @details Предоставляет различные конструкторы для создания буферов из строк или других буферов, а также вспомогательные функции для манипулирования данными, например, для проверки суффиксов.
class Buffer: public std::vector<uint8_t> {
public:
    DEFINE_CREATE(Buffer)

    Buffer() = default;

    /// \~english @brief Constructs a Buffer from a null-terminated C-style string.
    /// \~russian @brief Создает буфер из C-строки, завершающейся нулевым символом.
    /// \~english @param str The input C-style string.
    /// \~russian @param str Входная C-строка.
    Buffer(char const * const str);

    /// \~english @brief Constructs a Buffer from a std::string.
    /// \~russian @brief Создает буфер из std::string.
    /// \~english @param str The input std::string.
    /// \~russian @param str Входная строка std::string.
    Buffer(std::string const &str);

    /// \~english @brief Constructs a Buffer by concatenating a list of other buffers.
    /// \~russian @brief Создает буфер путем объединения списка других буферов.
    /// \~english @param buffers A list of shared pointers to buffers to be concatenated.
    /// \~russian @param buffers Список умных указателей на буферы для объединения.
    Buffer(std::list<Buffer::TSharedPtr> const &buffers);

    /// \~english @brief Constructs a Buffer by forwarding arguments to the std::vector constructor.
    /// \~english @details This allows for creating a buffer with various std::vector initializations (e.g., size, fill value, iterators).
    /// \~russian @brief Создает буфер, перенаправляя аргументы в конструктор std::vector.
    /// \~russian @details Это позволяет создавать буфер с различными вариантами инициализации std::vector (например, с указанием размера, значения для заполнения, итераторов).
    /// \~english @tparam ...TArgs Types of arguments for the std::vector constructor.
    /// \~russian @tparam ...TArgs Типы аргументов для конструктора std::vector.
    /// \~english @param ...args Arguments forwarded to the std::vector constructor.
    /// \~russian @param ...args Аргументы, перенаправляемые в конструктор std::vector.
    template<typename ... TArgs>
    Buffer(TArgs ... args);

    /// \~english @brief Checks if the buffer ends with a given suffix string, optionally skipping trailing characters.
    /// \~russian @brief Проверяет, заканчивается ли буфер указанным суффиксом, с возможностью пропуска завершающих символов.
    /// \~english @param suffix The suffix string to check for.
    /// \~russian @param suffix Строка-суффикс для проверки.
    /// \~english @param skip A string of characters to ignore at the end of the buffer before checking the suffix.
    /// \~russian @param skip Строка символов, которые следует игнорировать в конце буфера перед проверкой суффикса.
    /// \~english @return `true` if the buffer ends with the suffix, `false` otherwise.
    /// \~russian @return `true`, если буфер заканчивается суффиксом, иначе `false`.
    bool checkSuffixEqual(std::string const &suffix, std::string const &skip = "") const;

    /// \~english @brief Checks if the buffer ends with a given raw byte suffix.
    /// \~russian @brief Проверяет, заканчивается ли буфер указанным сырым байтовым суффиксом.
    /// \~english @param suffix A pointer to the raw byte array representing the suffix.
    /// \~russian @param suffix Указатель на массив сырых байт, представляющий суффикс.
    /// \~english @param size The size of the suffix array.
    /// \~russian @param size Размер массива-суффикса.
    /// \~english @return `true` if the buffer ends with the suffix, `false` otherwise.
    /// \~russian @return `true`, если буфер заканчивается суффиксом, иначе `false`.
    bool checkSuffixEqual(uint8_t const * const suffix, size_t const &size) const;

    /// \~english @brief Checks if the buffer ends with a given suffix vector.
    /// \~russian @brief Проверяет, заканчивается ли буфер указанным суффиксом в виде вектора.
    /// \~english @param suffix The vector of bytes representing the suffix.
    /// \~russian @param suffix Вектор байт, представляющий суффикс.
    /// \~english @return `true` if the buffer ends with the suffix, `false` otherwise.
    /// \~russian @return `true`, если буфер заканчивается суффиксом, иначе `false`.
    bool checkSuffixEqual(std::vector<uint8_t> const &suffix) const;

    /// \~english @brief Appends the content of another buffer to this one.
    /// \~russian @brief Добавляет содержимое другого буфера в конец текущего.
    /// \~english @param buffer A shared pointer to the buffer whose content will be appended.
    /// \~russian @param buffer Умный указатель на буфер, содержимое которого будет добавлено.
    void emplace_back(TSharedPtr const &);
};


template<typename ... TArgs>
Buffer::Buffer(TArgs ... args)
:
    std::vector<uint8_t>(args ...)
{}


} // io
} // iridium


DEFINE_CONVERT(iridium::io::Buffer, std::string)
DEFINE_CONVERT(std::string, iridium::io::Buffer)


#endif // HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
