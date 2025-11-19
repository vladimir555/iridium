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


INode::TSharedPtr parseFile(std::string const &file_name);
void composeFile(std::string const &file_name, INode::TSharedPtr const &root_node);


} // namespace iridium::parsing


#endif // HEADER_FILE_PARSER_8AEAABAE_12E8_4F2B_9880_CE659DC13896
