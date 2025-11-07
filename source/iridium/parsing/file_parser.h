// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Declares functions for parsing files into a node tree and composing a node tree into a file.
///     These functions provide high-level file I/O for structured data represented by `INode` objects.
/// \~russian @file
/// @brief Объявляет функции для разбора файлов в дерево узлов и формирования дерева узлов в файл.
///     Эти функции предоставляют высокоуровневый файловый ввод-вывод для структурированных данных, представленных объектами `INode`.

#ifndef HEADER_FILE_PARSER_8AEAABAE_12E8_4F2B_9880_CE659DC13896
#define HEADER_FILE_PARSER_8AEAABAE_12E8_4F2B_9880_CE659DC13896


#include <string>

#include "node.h"

#include "iridium/smart_ptr.h"


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
namespace parsing {

/// \~english @brief Parses a file and constructs a tree of nodes representing its content.
///     The specific parsing logic (e.g., for XML, JSON, custom formats) is determined by the underlying implementation,
///     which is not detailed here but is expected to handle file I/O and format interpretation.
/// \~russian @brief Разбирает файл и строит дерево узлов, представляющее его содержимое.
///     Конкретная логика разбора (например, для XML, JSON, пользовательских форматов) определяется базовой реализацией,
///     которая здесь не детализируется, но ожидается, что она будет обрабатывать файловый ввод-вывод и интерпретацию формата.
/// \~english @param file_name The path to the file to be parsed.
/// \~russian @param file_name Путь к файлу, который необходимо разобрать.
/// \~english @return A shared pointer to the root `INode` of the parsed node structure.
///     Returns `nullptr` or throws an exception if parsing fails (e.g., file not found, format error).
/// \~russian @return Разделяемый указатель на корневой узел `INode` разобранной структуры узлов.
///     Возвращает `nullptr` или выбрасывает исключение в случае сбоя разбора (например, файл не найден, ошибка формата).
/// \~english @exception std::runtime_error May be thrown if the file cannot be opened or if a parsing error occurs.
/// \~russian @exception std::runtime_error Может быть выброшено, если файл не может быть открыт или происходит ошибка разбора.
INode::TSharedPtr parseFile(std::string const &file_name);

/// \~english @brief Composes (serializes) a tree of nodes, starting from the given root node, into a file.
///     The specific composition logic (e.g., to XML, JSON, custom formats) is determined by the underlying implementation.
/// \~russian @brief Формирует (сериализует) дерево узлов, начиная с заданного корневого узла, в файл.
///     Конкретная логика формирования (например, в XML, JSON, пользовательские форматы) определяется базовой реализацией.
/// \~english @param file_name The path to the file where the node structure will be written.
/// \~russian @param file_name Путь к файлу, в который будет записана структура узлов.
/// \~english @param root_node A shared pointer to the root `INode` of the structure to be composed.
/// \~russian @param root_node Разделяемый указатель на корневой узел `INode` структуры, которую необходимо сформировать.
/// \~english @exception std::runtime_error May be thrown if the file cannot be written (e.g., permission issues)
///     or if the node structure is invalid or cannot be serialized to the target format.
/// \~russian @exception std::runtime_error Может быть выброшено, если файл не может быть записан (например, проблемы с правами доступа)
///     или если структура узлов недействительна или не может быть сериализована в целевой формат.
void composeFile(std::string const &file_name, INode::TSharedPtr const &root_node);


} // parsing
} // iridium


#endif // HEADER_FILE_PARSER_8AEAABAE_12E8_4F2B_9880_CE659DC13896
