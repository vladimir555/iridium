// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
#define HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1


#include "iridium/convertion/convert.h" // For DEFINE_CONVERT
#include "iridium/smart_ptr.h"       // For TSharedPtr and DEFINE_CREATE
#include <vector>
#include <string>
#include <list>


namespace iridium {
/// \~english @brief Namespace for Input/Output related functionalities.
/// \~russian @brief Пространство имен для функциональности, связанной с вводом-выводом.
namespace io {


/// \~english @brief A dynamic memory buffer for I/O operations, derived from `std::vector<unsigned char>`.
///     This class extends `std::vector<unsigned char>`, inheriting its methods for memory management,
///     element access, and iteration. It provides additional constructors for convenient
///     initialization from strings and lists of other buffers.
/// \~russian @brief Динамический буфер памяти для операций ввода-вывода, унаследованный от `std::vector<unsigned char>`.
///     Этот класс расширяет `std::vector<unsigned char>`, наследуя его методы для управления памятью,
///     доступа к элементам и итерации. Он предоставляет дополнительные конструкторы для удобной
///     инициализации из строк и списков других буферов.
class Buffer: public std::vector<unsigned char> {
public:
    /// \~english @brief Macro likely defining a static `create` method or similar factory functionality
    ///     for creating shared pointers (`Buffer::TSharedPtr`) to `Buffer` instances.
    /// \~russian @brief Макрос, вероятно, определяющий статический метод `create` или аналогичную фабричную функциональность
    ///     для создания умных указателей (`Buffer::TSharedPtr`) на экземпляры `Buffer`.
    DEFINE_CREATE(Buffer)

    /// \~english @brief Default constructor. Creates an empty buffer.
    ///     Inherits behavior from `std::vector<unsigned char>` default constructor.
    /// \~russian @brief Конструктор по умолчанию. Создает пустой буфер.
    ///     Наследует поведение от конструктора по умолчанию `std::vector<unsigned char>`.
    Buffer() = default;

    /// \~english @brief Constructs a Buffer from a null-terminated C-style string.
    ///     The buffer is initialized with a copy of the characters from the C-string, including the null terminator if counted by strlen.
    ///     The length is determined by `strlen`.
    /// \~russian @brief Конструирует Buffer из C-строки, завершающейся нулевым символом.
    ///     Буфер инициализируется копией символов из C-строки, включая нулевой терминатор, если он учитывается strlen.
    ///     Длина определяется с помощью `strlen`.
    /// \~english @param str A pointer to a constant null-terminated character array. Must not be null.
    /// \~russian @param str Указатель на константный символьный массив, завершающийся нулевым символом. Не должен быть null.
    Buffer(char const * const str);

    /// \~english @brief Constructs a Buffer from an `std::string`.
    ///     The buffer is initialized with a copy of the characters from the string.
    /// \~russian @brief Конструирует Buffer из `std::string`.
    ///     Буфер инициализируется копией символов из строки.
    /// \~english @param str The `std::string` to copy data from.
    /// \~russian @param str `std::string`, из которой копируются данные.
    Buffer(std::string const &str);

    /// \~english @brief Constructs a Buffer by concatenating a list of other Buffer shared pointers.
    ///     The new buffer's content is formed by appending the data from each buffer in the provided list.
    ///     The total capacity is reserved beforehand to avoid multiple reallocations.
    /// \~russian @brief Конструирует Buffer путем конкатенации списка умных указателей на другие Buffer.
    ///     Содержимое нового буфера формируется путем добавления данных из каждого буфера в предоставленном списке.
    ///     Общая емкость резервируется заранее, чтобы избежать многократных перераспределений памяти.
    /// \~english @param buffers A list of `Buffer::TSharedPtr` (shared pointers to Buffer objects) whose contents will be concatenated.
    /// \~russian @param buffers Список `Buffer::TSharedPtr` (умных указателей на объекты Buffer), содержимое которых будет объединено.
    Buffer(std::list<Buffer::TSharedPtr> const &buffers);

    /// \~english @brief Variadic template constructor forwarding arguments to `std::vector<unsigned char>`.
    ///     Allows initialization using any constructor of `std::vector<unsigned char>`, such as
    ///     `Buffer(size_t count, unsigned char value)` or constructing from iterators.
    /// \~russian @brief Вариативный шаблонный конструктор, перенаправляющий аргументы в `std::vector<unsigned char>`.
    ///     Позволяет инициализацию с использованием любого конструктора `std::vector<unsigned char>`, такого как
    ///     `Buffer(size_t count, unsigned char value)` или конструирование из итераторов.
    /// \~english @tparam TArgs Types of arguments to forward to the `std::vector` constructor.
    /// \~russian @tparam TArgs Типы аргументов для перенаправления в конструктор `std::vector`.
    /// \~english @param args Arguments to forward.
    /// \~russian @param args Аргументы для перенаправления.
    template<typename ... TArgs>
    Buffer(TArgs ... args)
    :
        std::vector<unsigned char>(args ...)
    {}
//    size_t findLast(std::string const &str); // This method is commented out in the source.
};


} // io
} // iridium


/// \~english @brief Declares a conversion specialization from `std::string` to `iridium::io::Buffer`.
///     Allows using `iridium::convertion::convert<iridium::io::Buffer>(std_string_value)`.
/// \~russian @brief Объявляет специализацию преобразования из `std::string` в `iridium::io::Buffer`.
///     Позволяет использовать `iridium::convertion::convert<iridium::io::Buffer>(std_string_value)`.
DEFINE_CONVERT(iridium::io::Buffer, std::string)

/// \~english @brief Declares a conversion specialization from `iridium::io::Buffer` to `std::string`.
///     Allows using `iridium::convertion::convert<std::string>(iridium_buffer_value)`.
/// \~russian @brief Объявляет специализацию преобразования из `iridium::io::Buffer` в `std::string`.
///     Позволяет использовать `iridium::convertion::convert<std::string>(iridium_buffer_value)`.
DEFINE_CONVERT(std::string, iridium::io::Buffer)


#endif // HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
