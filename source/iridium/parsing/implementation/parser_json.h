// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PARSER_JSON_C7A205BB_CD5F_4770_86BA_7DE37F7A5F88
#define HEADER_PARSER_JSON_C7A205BB_CD5F_4770_86BA_7DE37F7A5F88


#include "iridium/smart_ptr.h"
#include "iridium/parsing/node.h"
#include "iridium/parsing/parser.h"


namespace iridium {
/// \~english @brief Provides interfaces, utilities, and concrete classes for parsing and composing structured data.
/// \~russian @brief Предоставляет интерфейсы, утилиты и конкретные классы для разбора и формирования структурированных данных.
namespace parsing {
/// \~english @brief Contains concrete implementations of parsing components, such as node types and parsers.
/// \~russian @brief Содержит конкретные реализации компонентов разбора, таких как типы узлов и парсеры.
namespace implementation {

/// \~english @brief Concrete parser implementation for JSON (JavaScript Object Notation) data.
///     This class provides functionality to parse a JSON string into a structured `INode` tree,
///     and to compose an `INode` tree representing JSON data back into its string format.
/// \~russian @brief Конкретная реализация парсера для данных в формате JSON (JavaScript Object Notation).
///     Этот класс предоставляет функциональность для разбора строки JSON в структурированное дерево `INode`,
///     а также для формирования (компоновки) дерева `INode`, представляющего данные JSON,
///     обратно в его строковый формат.
class CJSONParser : public IParser {
public:
    /// \~english @brief Defines standard implementation helpers for `CJSONParser`, such as smart pointer typedefs and a static `create` method.
    /// \~russian @brief Определяет стандартные вспомогательные средства реализации для `CJSONParser`, такие как typedef-ы умных указателей и статический метод `create`.
    DEFINE_IMPLEMENTATION(CJSONParser)
    /// \~english @brief Default constructor.
    /// \~russian @brief Конструктор по умолчанию.
    CJSONParser() = default;
    /// \~english @brief Parses a JSON string into a node tree structure.
    ///     The resulting tree represents JSON objects, arrays, and values as nodes.
    /// \~russian @brief Разбирает строку JSON в древовидную структуру узлов.
    ///     Результирующее дерево представляет объекты, массивы и значения JSON в виде узлов.
    /// \~english @param source The JSON string to be parsed.
    /// \~russian @param source Строка JSON для разбора.
    /// \~english @return A shared pointer to the root `INode` of the parsed JSON structure.
    /// \~russian @return Разделяемый указатель на корневой узел `INode` разобранной структуры JSON.
    /// \~english @exception std::runtime_error May be thrown if the JSON string is malformed or a parsing error occurs.
    /// \~russian @exception std::runtime_error Может быть выброшено, если строка JSON некорректна или происходит ошибка разбора.
    INode::TSharedPtr parse(std::string const &source) const override;
    /// \~english @brief Composes a node tree structure representing JSON data into a JSON string.
    /// \~russian @brief Формирует (компонует) древовидную структуру узлов, представляющую данные JSON, в строку JSON.
    /// \~english @param root_node A const shared pointer to the root `INode` of the JSON structure to be composed.
    /// \~russian @param root_node Константный разделяемый указатель на корневой узел `INode` структуры JSON для компоновки.
    /// \~english @return A string containing the JSON representation of the node tree.
    /// \~russian @return Строка, содержащая представление дерева узлов в формате JSON.
    /// \~english @exception std::runtime_error May be thrown if the node structure is not valid for composition into a JSON string.
    /// \~russian @exception std::runtime_error Может быть выброшено, если структура узлов недействительна для компоновки в строку JSON.
    std::string compose(INode::TConstSharedPtr const &root_node) const override;
};


} // implementation
} // parsing
} // iridium


#endif // HEADER_PARSER_JSON_C7A205BB_CD5F_4770_86BA_7DE37F7A5F88
