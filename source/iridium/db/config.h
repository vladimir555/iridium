// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892
#define HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892


/// \~english @file
/// @brief Defines the database configuration structure.
/// \~russian @file
/// @brief Определяет структуру конфигурации базы данных.


#include "iridium/parsing/serialization/node.h"
#include "iridium/enum.h"


namespace iridium::db::config {


    /// \~english @brief Defines the structure for database configuration.
    /// \~english This structure is designed to be serialized from/to a configuration file (e.g., JSON).
    /// \~english It holds all the necessary parameters to establish a database connection.
    /// \~russian @brief Определяет структуру для конфигурации базы данных.
    /// \~russian Эта структура предназначена для сериализации из/в конфигурационный файл (например, JSON).
    /// \~russian Она содержит все необходимые параметры для установки соединения с базой данных.
    DEFINE_ROOT_NODE_BEGIN(Datebase)
        /// \~english @brief Enumeration for the supported database types.
        /// \~russian @brief Перечисление для поддерживаемых типов баз данных.
        DEFINE_ENUM(
            TDBType,
            MYSQL,    ///< \~english MySQL database. \~russian База данных MySQL.
            POSTGRES  ///< \~english PostgreSQL database. \~russian База данных PostgreSQL.
        );

        /// \~english @brief The type of the database (e.g., MYSQL, POSTGRES).
        /// \~russian @brief Тип базы данных (например, MYSQL, POSTGRES).
        DEFINE_ATTRIBUTE(TDBType, Type, {})

        /// \~english @brief The hostname or IP address of the database server.
        /// \~russian @brief Имя хоста или IP-адрес сервера базы данных.
        DEFINE_ATTRIBUTE(std::string, Host, {})

        /// \~english @brief The port number of the database server.
        /// \~russian @brief Номер порта сервера базы данных.
        DEFINE_ATTRIBUTE(uint16_t, Port, {})

        /// \~english @brief The username for authentication.
        /// \~russian @brief Имя пользователя для аутентификации.
        DEFINE_ATTRIBUTE(std::string, User, {})

        /// \~english @brief The password for authentication.
        /// \~russian @brief Пароль для аутентификации.
        DEFINE_ATTRIBUTE(std::string, Password, {})

        /// \~english @brief The name of the database to connect to.
        /// \~russian @brief Имя базы данных для подключения.
        DEFINE_ATTRIBUTE(std::string, Database, {})
    DEFINE_ROOT_NODE_END()


} // namespace iridium::db::config


DEFINE_ENUM_CONVERT(iridium::db::config::TDatebase::TDBType)


#endif // HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892
