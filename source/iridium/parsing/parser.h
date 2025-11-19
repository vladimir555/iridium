// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `IParser` interface for parsing and composing structured data.
///     This interface provides methods to convert between string representations and `INode` tree structures.
/// \~russian @file
/// @brief Определяет интерфейс `IParser` для разбора и формирования структурированных данных.
///     Этот интерфейс предоставляет методы для преобразования между строковыми представлениями и древовидными структурами `INode`.

#ifndef HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880
#define HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880


#include <string>

#include "iridium/smart_ptr.h"

#include "node.h"


namespace iridium::parsing {


class IParser {
public:
    /// \~english @brief Defines standard smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.) for the `IParser` interface.
    /// \~russian @brief Определяет стандартные typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.) для интерфейса `IParser`.
    DEFINE_SMART_PTR(IParser)
    virtual ~IParser() = default;

    virtual INode::TSharedPtr parse(std::string const &source) const = 0;
    virtual std::string compose(INode::TConstSharedPtr const &root_node) const = 0;
};


} // namespace iridium::parsing


#endif // HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880
