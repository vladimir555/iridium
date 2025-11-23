// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155
#define HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155


/// \~english @file
/// @brief Defines the `CMySQLConnector` class for connecting to a MySQL database.
/// \~russian @file
/// @brief Определяет класс `CMySQLConnector` для подключения к базе данных MySQL.


#include "iridium/build_flags.h"
#ifdef BUILD_FLAG_MYSQL


#include "iridium/smart_ptr.h"
#include "iridium/db/connector.h"
#include "iridium/io/uri.h"

#include "connector.h"

#include <string>
#include <mysql.h>


namespace iridium::db::implementation {


/// \~english @brief A concrete implementation of the `IConnector` interface for MySQL.
/// \~russian @brief Конкретная реализация интерфейса `IConnector` для MySQL.
class CMySQLConnector:
    public IConnector,
    public CConnector
{
public:
    DEFINE_CREATE(CMySQLConnector)

    /// \~english @brief Constructs a `CMySQLConnector` with the given database configuration.
    /// \~russian @brief Конструирует `CMySQLConnector` с заданной конфигурацией базы данных.
    CMySQLConnector(config::TDatebase const &config);

    /// \~english @brief Destroys the `CMySQLConnector` object.
    /// \~russian @brief Уничтожает объект `CMySQLConnector`.
    virtual ~CMySQLConnector();

    /// \~english @brief Initializes the connection to the database.
    /// \~russian @brief Инициализирует соединение с базой данных.
    void initialize() override;

    /// \~english @brief Finalizes the connection to the database.
    /// \~russian @brief Завершает соединение с базой данных.
    void finalize() override;

    /// \~english @brief Sends a query to the database.
    /// \~russian @brief Отправляет запрос к базе данных.
    INode::TSharedPtr sendQuery(std::string const &query) override;

private:
    ::MYSQL m_connection;
};


} // iridium::db::implementation


#endif // COMPILATION_FLAG_MYSQL


#endif // HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155
