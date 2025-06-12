// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PARSER_XML_EF217266_E91A_4908_97D1_2BB5420DFF5A
#define HEADER_PARSER_XML_EF217266_E91A_4908_97D1_2BB5420DFF5A


#include "iridium/smart_ptr.h"
#include "iridium/parsing/node.h"
#include "iridium/parsing/parser.h"


namespace iridium {
namespace parsing {
namespace implementation {

/// \~english @brief Concrete parser implementation for XML (eXtensible Markup Language) data.
///     This class provides functionality to parse an XML string into a structured `INode` tree,
///     representing XML elements, attributes, and text content. It can also compose an `INode`
///     tree back into its XML string format.
/// \~russian @brief Конкретная реализация парсера для данных в формате XML (eXtensible Markup Language).
///     Этот класс предоставляет функциональность для разбора строки XML в структурированное дерево `INode`,
///     представляя элементы XML, атрибуты и текстовое содержимое. Он также может формировать (компоновать)
///     дерево `INode` обратно в его строковый формат XML.
class CXMLParser: public IParser {
public:
    /// \~english @brief Defines standard implementation helpers for `CXMLParser`, such as smart pointer typedefs and a static `create` method.
    /// \~russian @brief Определяет стандартные вспомогательные средства реализации для `CXMLParser`, такие как typedef-ы умных указателей и статический метод `create`.
    DEFINE_IMPLEMENTATION(CXMLParser)
    /// \~english @brief Default constructor.
    /// \~russian @brief Конструктор по умолчанию.
    CXMLParser() = default;
    /// \~english @brief Parses an XML string into a node tree structure.
    ///     The resulting tree represents XML elements as nodes, with attributes and text content accessible via the node interface.
    /// \~russian @brief Разбирает строку XML в древовидную структуру узлов.
    ///     Результирующее дерево представляет элементы XML как узлы, с атрибутами и текстовым содержимым, доступными через интерфейс узла.
    /// \~english @param source The XML string to be parsed.
    /// \~russian @param source Строка XML для разбора.
    /// \~english @return A shared pointer to the root `INode` of the parsed XML structure.
    /// \~russian @return Разделяемый указатель на корневой узел `INode` разобранной структуры XML.
    /// \~english @exception std::runtime_error May be thrown if the XML string is malformed or a parsing error occurs.
    /// \~russian @exception std::runtime_error Может быть выброшено, если строка XML некорректна или происходит ошибка разбора.
    INode::TSharedPtr parse(std::string const &source) const override;
    /// \~english @brief Composes a node tree structure representing XML data into an XML string.
    /// \~russian @brief Формирует (компонует) древовидную структуру узлов, представляющую данные XML, в строку XML.
    /// \~english @param root_node A const shared pointer to the root `INode` of the XML structure to be composed.
    /// \~russian @param root_node Константный разделяемый указатель на корневой узел `INode` структуры XML для компоновки.
    /// \~english @return A string containing the XML representation of the node tree.
    /// \~russian @return Строка, содержащая представление дерева узлов в формате XML.
    /// \~english @exception std::runtime_error May be thrown if the node structure is not valid for composition into an XML string (e.g., invalid XML characters in names/values without proper handling by implementation).
    /// \~russian @exception std::runtime_error Может быть выброшено, если структура узлов недействительна для компоновки в строку XML (например, недопустимые символы XML в именах/значениях без надлежащей обработки реализацией).
    std::string compose(INode::TConstSharedPtr const &root_node) const override;
};


} // implementation
} // parsing
} // iridium


#endif // HEADER_PARSER_XML_EF217266_E91A_4908_97D1_2BB5420DFF5A
