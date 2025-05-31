// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NODE_C3E75C97_C541_49C8_AEF6_4D6E2953B0E4
#define HEADER_NODE_C3E75C97_C541_49C8_AEF6_4D6E2953B0E4


#include "node_type.h"


namespace iridium {
namespace parsing {
/// \~english @brief Contains concrete implementations of parsing components, such as node types and parsers.
///     This namespace provides the actual logic for parsing and composing data structures defined by interfaces in the parent `iridium::parsing` namespace.
/// \~russian @brief Содержит конкретные реализации компонентов разбора, таких как типы узлов и парсеры.
///     Это пространство имен предоставляет фактическую логику для разбора и формирования структур данных, определенных интерфейсами в родительском пространстве имен `iridium::parsing`.
namespace implementation {

/// \~english @typedef CNode
/// @brief Defines `CNode` as a specialization of the concrete `CNodeType` class template with `std::string` as its value type.
///     This is likely the default concrete node class used by various parsers within the Iridium framework
///     to build a tree representation of parsed data (e.g., from configuration files).
/// \~russian @typedef CNode
/// @brief Определяет `CNode` как специализацию конкретного класса-шаблона `CNodeType` с `std::string` в качестве типа значения.
///     Вероятно, это класс конкретного узла по умолчанию, используемый различными парсерами во фреймворке Iridium
///     для построения древовидного представления разобранных данных (например, из конфигурационных файлов).
typedef CNodeType<std::string> CNode;


} // implementation
} // parsing
} // iridium


#endif // HEADER_NODE_C3E75C97_C541_49C8_AEF6_4D6E2953B0E4
