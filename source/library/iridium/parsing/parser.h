// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880
#define HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880


#include <string>

#include "iridium/smart_ptr.h"

#include "node.h"


namespace iridium::parsing {


/// \~english @brief Interface for parsing and composing data (serialization/deserialization).
/// \~russian @brief Интерфейс для парсинга и формирования данных (сериализация/десериализация).
class IParser {
public:
    DEFINE_SMART_PTR(IParser)
    virtual ~IParser() = default;

    /// \~english @brief Parses a string into a node tree.
    /// \~russian @brief Парсит строку в дерево узлов.
    virtual INode::TSharedPtr parse(std::string const &source) const = 0;
    /// \~english @brief Composes a node tree into its string representation.
    /// \~russian @brief Преобразует дерево узлов в его строковое представление.
    virtual std::string compose(INode::TConstSharedPtr const &root_node) const = 0;
};


} // namespace iridium::parsing


#endif // HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880
