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


/// \~english @brief Interface for a generic parser.
/// \~english @details A parser is responsible for converting a string representation of structured data into a tree of INode objects (parsing) and vice versa (composing).
/// \~russian @brief Интерфейс для обобщенного парсера.
/// \~russian @details Парсер отвечает за преобразование строкового представления структурированных данных в дерево объектов INode (разбор) и обратно (формирование).
class IParser {
public:
    /// \~english @brief Defines standard smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.) for the `IParser` interface.
    /// \~russian @brief Определяет стандартные typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.) для интерфейса `IParser`.
    DEFINE_SMART_PTR(IParser)
    virtual ~IParser() = default;

    /// \~english @brief Parses a string into a tree of nodes.
    /// \~russian @brief Разбирает строку в дерево узлов.
    /// \~english @param source The string to parse.
    /// \~russian @param source Строка для разбора.
    /// \~english @return A shared pointer to the root node of the parsed tree.
    /// \~russian @return Умный указатель на корневой узел разобранного дерева.
    virtual INode::TSharedPtr parse(std::string const &source) const = 0;

    /// \~english @brief Composes a tree of nodes into a string.
    /// \~russian @brief Формирует дерево узлов в строку.
    /// \~english @param root_node The root node of the tree to compose.
    /// \~russian @param root_node Корневой узел дерева для формирования.
    /// \~english @return A string representation of the node tree.
    /// \~russian @return Строковое представление дерева узлов.
    virtual std::string compose(INode::TConstSharedPtr const &root_node) const = 0;
};


} // namespace iridium::parsing


#endif // HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880
