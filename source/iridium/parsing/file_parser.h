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


namespace iridium::parsing {


/// \~english @brief Parses a file and constructs a tree of `INode` objects representing its structure.
/// \~russian @brief Разбирает файл и строит дерево объектов `INode`, представляющее его структуру.
/// \~english @param file_name The path to the file to be parsed.
/// \~russian @param file_name Путь к файлу для разбора.
/// \~english @return A shared pointer to the root `INode` of the parsed tree.
/// \~russian @return Умный указатель на корневой `INode` разобранного дерева.
/// \~english @throws std::runtime_error or similar if the file cannot be read or if parsing fails.
/// \~russian @throws std::runtime_error или подобное исключение, если файл не может быть прочитан или разбор завершается неудачей.
INode::TSharedPtr parseFile(std::string const &file_name);

/// \~english @brief Composes a tree of `INode` objects into a file, serializing its structure.
/// \~russian @brief Формирует дерево объектов `INode` в файл, сериализуя его структуру.
/// \~english @param file_name The path to the file where the output should be written.
/// \~russian @param file_name Путь к файлу, в который следует записать результат.
/// \~english @param root_node A shared pointer to the root `INode` of the tree to be composed.
/// \~russian @param root_node Умный указатель на корневой `INode` дерева для формирования.
/// \~english @throws std::runtime_error or similar if the file cannot be written to or if composition fails.
/// \~russian @throws std::runtime_error или подобное исключение, если в файл невозможно выполнить запись или формирование завершается неудачей.
void composeFile(std::string const &file_name, INode::TSharedPtr const &root_node);


} // namespace iridium::parsing


#endif // HEADER_FILE_PARSER_8AEAABAE_12E8_4F2B_9880_CE659DC13896
