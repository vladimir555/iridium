// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `CConnector` base class for concrete database connector implementations.
/// \~russian @file
/// @brief Определяет базовый класс `CConnector` для конкретных реализаций коннекторов баз данных.

#ifndef HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D
#define HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D


#include "iridium/pattern/non_copyable.h"
#include "iridium/db/config.h"


namespace iridium::db::implementation {


/// \~english @brief Base class for database connector implementations.
///     This class provides common functionality, such as storing database configuration,
///     and ensures that connector objects are non-copyable. Concrete database connectors
///     (e.g., for MySQL, PostgreSQL) are expected to derive from this class.
/// \~russian @brief Базовый класс для реализаций коннекторов баз данных.
///     Этот класс предоставляет общую функциональность, такую как хранение конфигурации базы данных,
///     и гарантирует, что объекты коннекторов не могут быть скопированы. Предполагается, что конкретные
///     коннекторы баз данных (например, для MySQL, PostgreSQL) будут наследоваться от этого класса.
class CConnector : public pattern::NonCopyable {
public:
    /// \~english @brief Constructs a CConnector with the given database configuration.
    ///     Initializes the connector with the necessary parameters to establish a database connection.
    /// \~russian @brief Конструирует CConnector с заданной конфигурацией базы данных.
    ///     Инициализирует коннектор необходимыми параметрами для установки соединения с базой данных.
    /// \~english @param config The database configuration object containing connection details
    ///     (e.g., host, port, user, password, database type).
    /// \~russian @param config Объект конфигурации базы данных, содержащий детали подключения
    ///     (например, хост, порт, пользователь, пароль, тип базы данных).
    CConnector(config::TDatebase const &config);

    /// \~english @brief Virtual default destructor.
    ///     Ensures proper cleanup when derived connector objects are deleted via a base class pointer.
    /// \~russian @brief Виртуальный деструктор по умолчанию.
    ///     Обеспечивает корректное освобождение ресурсов при удалении объектов производных коннекторов через указатель на базовый класс.
    virtual ~CConnector() = default;

protected:
    /// \~english @brief Stores the database configuration for this connector instance.
    ///     Derived classes can use this configuration to manage connection parameters.
    /// \~russian @brief Хранит конфигурацию базы данных для этого экземпляра коннектора.
    ///     Производные классы могут использовать эту конфигурацию для управления параметрами подключения.
    config::TDatebase m_config;
};


} // iridium::db::implementation


#endif // HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D
