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


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
namespace parsing {

/// \~english @brief Interface for parsers and composers (serializers) of data.
///     Parsers are responsible for converting a string representation of data (e.g., XML, JSON, custom format)
///     into a tree of `INode` objects. Composers perform the reverse operation, converting a tree
///     of `INode` objects back into a string representation.
/// \~russian @brief Интерфейс для парсеров и компоновщиков (сериализаторов) данных.
///     Парсеры отвечают за преобразование строкового представления данных (например, XML, JSON, пользовательский формат)
///     в дерево объектов `INode`. Компоновщики выполняют обратную операцию, преобразуя
///     дерево объектов `INode` обратно в строковое представление.
class IParser {
public:
    /// \~english @brief Defines standard smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.) for the `IParser` interface.
    /// \~russian @brief Определяет стандартные typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.) для интерфейса `IParser`.
    DEFINE_SMART_PTR(IParser)
    /// \~english @brief Virtual destructor to ensure proper cleanup of derived class resources.
    /// \~russian @brief Виртуальный деструктор для обеспечения корректного освобождения ресурсов производного класса.
    virtual ~IParser() = default;
    /// \~english @brief Parses a source string and constructs a tree of nodes representing its content.
    /// \~russian @brief Разбирает исходную строку и строит дерево узлов, представляющее ее содержимое.
    /// \~english @param source The string data to be parsed.
    /// \~russian @param source Строковые данные для разбора.
    /// \~english @return A shared pointer to the root `INode` of the parsed node structure.
    ///     Returns `nullptr` or throws an exception if parsing fails (e.g., format error).
    /// \~russian @return Разделяемый указатель на корневой узел `INode` разобранной структуры узлов.
    ///     Возвращает `nullptr` или выбрасывает исключение в случае сбоя разбора (например, ошибка формата).
    /// \~english @exception std::runtime_error May be thrown if a parsing error occurs.
    /// \~russian @exception std::runtime_error Может быть выброшено при возникновении ошибки разбора.
    virtual INode::TSharedPtr parse(std::string const &source) const = 0;
    /// \~english @brief Composes (serializes) a tree of nodes, starting from the given root node, into a string representation.
    /// \~russian @brief Формирует (сериализует) дерево узлов, начиная с заданного корневого узла, в строковое представление.
    /// \~english @param root_node A const shared pointer to the root `INode` of the structure to be composed.
    /// \~russian @param root_node Константный разделяемый указатель на корневой узел `INode` структуры, которую необходимо сформировать.
    /// \~english @return A string containing the serialized representation of the node tree.
    /// \~russian @return Строка, содержащая сериализованное представление дерева узлов.
    /// \~english @exception std::runtime_error May be thrown if the node structure is invalid or cannot be serialized.
    /// \~russian @exception std::runtime_error Может быть выброшено, если структура узлов недействительна или не может быть сериализована.
    virtual std::string compose(INode::TConstSharedPtr const &root_node) const = 0;
};


} // parsing
} // iridium


#endif // HEADER_PARSER_03279B58_63F8_4528_B88B_B391B1E43880
