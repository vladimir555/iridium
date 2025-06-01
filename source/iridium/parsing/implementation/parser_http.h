// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PARSER_HTTP_DD6F8041_66CA_40B5_9844_D362F29F1616
#define HEADER_PARSER_HTTP_DD6F8041_66CA_40B5_9844_D362F29F1616


#include "iridium/smart_ptr.h"
#include "iridium/parsing/node.h"
#include "iridium/parsing/parser.h"


namespace iridium {
namespace parsing {
namespace implementation {

/// \~english @brief Concrete parser implementation for HTTP messages.
///     This class provides functionality to parse raw HTTP request or response strings
///     into a structured `INode` tree, and to compose an `INode` tree representing an
///     HTTP message back into its raw string format.
/// \~russian @brief Конкретная реализация парсера для HTTP-сообщений.
///     Этот класс предоставляет функциональность для разбора необработанных строк HTTP-запросов или ответов
///     в структурированное дерево `INode`, а также для формирования (компоновки) дерева `INode`,
///     представляющего HTTP-сообщение, обратно в его необработанный строковый формат.
class CHTTPParser: public IParser {
public:
    /// \~english @brief Defines standard implementation helpers for `CHTTPParser`, such as smart pointer typedefs and a static `create` method.
    /// \~russian @brief Определяет стандартные вспомогательные средства реализации для `CHTTPParser`, такие как typedef-ы умных указателей и статический метод `create`.
    DEFINE_IMPLEMENTATION(CHTTPParser)
    /// \~english @brief Default constructor.
    /// \~russian @brief Конструктор по умолчанию.
    CHTTPParser() = default;
    /// \~english @brief Parses an HTTP message string into a node tree structure.
    ///     The resulting tree typically represents headers, body, and other HTTP components as nodes.
    /// \~russian @brief Разбирает строку HTTP-сообщения в древовидную структуру узлов.
    ///     Результирующее дерево обычно представляет заголовки, тело и другие компоненты HTTP в виде узлов.
    /// \~english @param source The raw HTTP message string (request or response) to be parsed.
    /// \~russian @param source Необработанная строка HTTP-сообщения (запрос или ответ) для разбора.
    /// \~english @return A shared pointer to the root `INode` of the parsed HTTP message structure.
    /// \~russian @return Разделяемый указатель на корневой узел `INode` разобранной структуры HTTP-сообщения.
    /// \~english @exception std::runtime_error May be thrown if the HTTP message is malformed or a parsing error occurs.
    /// \~russian @exception std::runtime_error Может быть выброшено, если HTTP-сообщение некорректно или происходит ошибка разбора.
    INode::TSharedPtr parse(std::string const &source) const override;
    /// \~english @brief Composes a node tree structure representing an HTTP message into a raw HTTP message string.
    /// \~russian @brief Формирует (компонует) древовидную структуру узлов, представляющую HTTP-сообщение, в необработанную строку HTTP-сообщения.
    /// \~english @param root_node A const shared pointer to the root `INode` of the HTTP message structure to be composed.
    /// \~russian @param root_node Константный разделяемый указатель на корневой узел `INode` структуры HTTP-сообщения для компоновки.
    /// \~english @return A string containing the raw HTTP message.
    /// \~russian @return Строка, содержащая необработанное HTTP-сообщение.
    /// \~english @exception std::runtime_error May be thrown if the node structure is not valid for composition into an HTTP message.
    /// \~russian @exception std::runtime_error Может быть выброшено, если структура узлов недействительна для компоновки в HTTP-сообщение.
    std::string compose(INode::TConstSharedPtr const &root_node) const override;
};


} // implementation
} // parsing
} // iridium


#endif // HEADER_PARSER_HTTP_DD6F8041_66CA_40B5_9844_D362F29F1616
