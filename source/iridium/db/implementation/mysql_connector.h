// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155
#define HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155


#include "iridium/build_flags.h"
#ifdef BUILD_FLAG_MYSQL


#include "iridium/smart_ptr.h"
#include "iridium/db/connector.h"
#include "iridium/io/uri.h"

#include "connector.h"

#include <string>
#include <mysql.h>


namespace iridium {
namespace db {
namespace implementation {


class CMySQLConnector :
    public IConnector,
    public CConnector
{
public:
    DEFINE_CREATE(CMySQLConnector)

    /// \~english @brief Constructs a MySQL connector.
    /// \~russian @brief Конструктор коннектора MySQL.
    /// \~english @param config The database configuration.
    /// \~russian @param config Конфигурация базы данных.
    CMySQLConnector(config::TDatebase const & config);

    /// \~english @brief Destructor.
    /// \~russian @brief Деструктор.
    virtual ~CMySQLConnector();

    /// \~english @brief Initializes the connection to the MySQL database.
    /// \~russian @brief Инициализирует соединение с базой данных MySQL.
    void initialize() override;

    /// \~english @brief Finalizes the connection to the MySQL database.
    /// \~russian @brief Завершает соединение с базой данных MySQL.
    void finalize() override;

    /// \~english @brief Sends a query to the MySQL database.
    /// \~russian @brief Отправляет запрос к базе данных MySQL.
    /// \~english @param query The SQL query to execute.
    /// \~russian @param query SQL-запрос для выполнения.
    /// \~english @return The result of the query as a tree of nodes.
    /// \~russian @return Результат запроса в виде дерева узлов.
    INode::TSharedPtr
        sendQuery(std::string const & query) override;

private:
    /// \~english @brief The MySQL connection handle.
    /// \~russian @brief Указатель на соединение MySQL.
    ::MYSQL m_connection;
};


} // implementation
} // db
} // iridium


#endif // COMPILATION_FLAG_MYSQL


#endif // HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155
