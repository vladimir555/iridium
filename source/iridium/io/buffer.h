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


namespace iridium::io {


/// \~english @brief A Buffer class for handling raw binary data, inheriting from std::vector<uint8_t>.
/// \~english @details Provides various constructors to create buffers from strings or other buffers, and utility functions for data manipulation, such as checking suffixes.
/// \~russian @brief Класс Buffer для работы с сырыми двоичными данными, унаследованный от std::vector<uint8_t>.
/// \~russian @details Предоставляет различные конструкторы для создания буферов из строк или других буферов, а также вспомогательные функции для манипулирования данными, например, для проверки суффиксов.
class Buffer: public std::vector<uint8_t> {
public:
    DEFINE_CREATE(Buffer)

    Buffer() = default;
    Buffer(char const * const str);
    Buffer(std::string const &str);
    Buffer(std::list<Buffer::TSharedPtr> const &buffers);

    template<typename ... TArgs>
    Buffer(TArgs ... args);

    bool checkSuffixEqual(std::string const &suffix, std::string const &skip = "") const;
    bool checkSuffixEqual(uint8_t const * const suffix, size_t const &size) const;
    bool checkSuffixEqual(std::vector<uint8_t> const &suffix) const;
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
