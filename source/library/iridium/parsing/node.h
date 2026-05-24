// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `INode` typedef, a specialization of `INodeType<std::string>`.
///     This serves as the primary type for representing nodes in parsed data structures
///     within the Iridium parsing framework. Also declares a conversion from `INode` to `std::string`.
/// \~russian @file
/// @brief Определяет typedef `INode`, специализацию `INodeType<std::string>`.
///     Служит основным типом для представления узлов в разобранных структурах данных
///     во фреймворке разбора Iridium. Также объявляет преобразование из `INode` в `std::string`.

#ifndef HEADER_NODE_7F5A86D1_A48E_4F8A_9779_D7495B3528A3
#define  HEADER_NODE_7F5A86D1_A48E_4F8A_9779_D7495B3528A3


#include "node_type.h"


namespace iridium::parsing {

/// \~english @typedef INode
/// @brief Defines `INode` as a specialization of the `INodeType` interface with `std::string` as its value type.
///     This typically represents a generic node within a parsed data structure (like a configuration file or XML/JSON document),
///     where each node can have a name, attributes, child nodes, and a string value.
/// \~russian @typedef INode
/// @brief Определяет `INode` как специализацию интерфейса `INodeType` с `std::string` в качестве типа значения.
///     Обычно представляет собой обобщенный узел в разобранной структуре данных (например, конфигурационный файл или документ XML/JSON),
///     где каждый узел может иметь имя, атрибуты, дочерние узлы и строковое значение.
typedef INodeType<std::string> INode;


} // namespace iridium::parsing

/// \~english @brief Declares a conversion specialization allowing an `iridium::parsing::INode` object (or rather, a shared pointer to it, as per typical usage) to be converted to a `std::string`.
///     This conversion likely extracts the primary string value associated with the node.
///     The actual implementation of this conversion is provided elsewhere (usually via `IMPLEMENT_CONVERT`).
/// \~russian @brief Объявляет специализацию преобразования, позволяющую объекту `iridium::parsing::INode` (или, скорее, разделяемому указателю на него, согласно типичному использованию) быть преобразованным в `std::string`.
///     Это преобразование, вероятно, извлекает основное строковое значение, связанное с узлом.
///     Фактическая реализация этого преобразования предоставляется в другом месте (обычно через `IMPLEMENT_CONVERT`).
DEFINE_CONVERT(std::string, iridium::parsing::INode)


#endif // HEADER_NODE_7F5A86D1_A48E_4F8A_9779_D7495B3528A3
