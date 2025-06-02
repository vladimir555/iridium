// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines database configuration structures using a serialization framework.
///     This header specifies the structure `Datebase` (intended as Database) for holding
///     database connection parameters like type, host, port, credentials, and database name.
/// \~russian @file
/// @brief Определяет структуры конфигурации базы данных с использованием фреймворка сериализации.
///     Этот заголовочный файл определяет структуру `Datebase` (предположительно Database) для хранения
///     параметров подключения к базе данных, таких как тип, хост, порт, учетные данные и имя базы данных.

#ifndef HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892
#define HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892


#include "iridium/parsing/serialization/node.h"
#include "iridium/enum.h"


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
/// \~english @brief Namespace for database related functionalities.
/// \~russian @brief Пространство имен для функциональности, связанной с базами данных.
namespace db {
/// \~english @brief Namespace for database configuration structures and settings.
/// \~russian @brief Пространство имен для структур конфигурации и настроек баз данных.
namespace config {


/// \~english @brief Defines the root structure for database configuration parameters.
///     This structure encapsulates all necessary information to establish a database connection.
///     It appears to be defined using a serialization framework (based on `DEFINE_ROOT_NODE_BEGIN/END` macros).
///     Note: The name "Datebase" seems to be a typo and likely intended to be "Database".
/// \~russian @brief Определяет корневую структуру для параметров конфигурации базы данных.
///     Эта структура инкапсулирует всю необходимую информацию для установки соединения с базой данных.
///     По-видимому, она определена с использованием фреймворка сериализации (на основе макросов `DEFINE_ROOT_NODE_BEGIN/END`).
///     Примечание: Имя "Datebase", похоже, является опечаткой и, вероятно, должно было быть "Database".
DEFINE_ROOT_NODE_BEGIN(Datebase)
    /// \~english @brief Enumeration for supported database types.
    /// \~russian @brief Перечисление для поддерживаемых типов баз данных.
    DEFINE_ENUM(
        TDBType,
        /// \~english @brief Represents a MySQL database type.
        /// \~russian @brief Представляет тип базы данных MySQL.
        MYSQL,
        /// \~english @brief Represents a PostgreSQL database type.
        /// \~russian @brief Представляет тип базы данных PostgreSQL.
        POSTGRES
    )
    /// \~english @brief The type of the database (e.g., MYSQL, POSTGRES).
    /// \~russian @brief Тип базы данных (например, MYSQL, POSTGRES).
    DEFINE_ATTRIBUTE(TDBType    , Type, {})
    /// \~english @brief The hostname or IP address of the database server.
    /// \~russian @brief Имя хоста или IP-адрес сервера базы данных.
    DEFINE_ATTRIBUTE(std::string, Host, {})
    /// \~english @brief The port number on which the database server is listening.
    /// \~russian @brief Номер порта, на котором прослушивает сервер базы данных.
    DEFINE_ATTRIBUTE(uint16_t   , Port, {})
    /// \~english @brief The username for authenticating with the database server.
    /// \~russian @brief Имя пользователя для аутентификации на сервере базы данных.
    DEFINE_ATTRIBUTE(std::string, User, {})
    /// \~english @brief The password for authenticating with the database server.
    /// \~russian @brief Пароль для аутентификации на сервере базы данных.
    DEFINE_ATTRIBUTE(std::string, Password, {})
    /// \~english @brief The name of the specific database/schema to connect to on the server.
    /// \~russian @brief Имя конкретной базы данных/схемы для подключения на сервере.
    DEFINE_ATTRIBUTE(std::string, Database, {})
DEFINE_ROOT_NODE_END()


} // config


} // db
} // iridium


#endif // HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892
