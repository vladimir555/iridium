// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `CNode` typedef, a concrete node implementation based on `CNodeType<std::string>`.
///     This serves as the default concrete node type for the Iridium parsing framework.
/// \~russian @file
/// @brief Определяет typedef `CNode`, конкретную реализацию узла на основе `CNodeType<std::string>`.
///     Служит конкретным типом узла по умолчанию для фреймворка разбора Iridium.

#ifndef HEADER_NODE_C3E75C97_C541_49C8_AEF6_4D6E2953B0E4
#define HEADER_NODE_C3E75C97_C541_49C8_AEF6_4D6E2953B0E4


#include "node_type.h"


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
/// \~english @brief Provides interfaces, utilities, and concrete classes for parsing and composing structured data.
///     (Implementation details are in the `implementation` sub-namespace).
/// \~russian @brief Предоставляет интерфейсы, утилиты и конкретные классы для разбора и формирования структурированных данных.
///     (Детали реализации находятся в подпространстве имен `implementation`).
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
