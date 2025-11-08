// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C
#define HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C


#include "iridium/build_flags.h"
#ifdef BUILD_FLAG_POSTGRES


#include "iridium/smart_ptr.h"
#include "iridium/db/connector.h"
#include "iridium/io/uri.h"

#include "connector.h"

#include <string>
#include <libpq-fe.h>


namespace iridium {
namespace db {
namespace implementation {


class CPostgresConnector :
    public IConnector,
    public CConnector
{
public:
    DEFINE_CREATE(CPostgresConnector)

    /// \~english @brief Constructs a PostgreSQL connector.
    /// \~russian @brief Конструктор коннектора PostgreSQL.
    /// \~english @param config The database configuration.
    /// \~russian @param config Конфигурация базы данных.
    CPostgresConnector(config::TDatebase const &config);

    /// \~english @brief Destructor.
    /// \~russian @brief Деструктор.
    virtual ~CPostgresConnector() override;

    /// \~english @brief Initializes the connection to the PostgreSQL database.
    /// \~russian @brief Инициализирует соединение с базой данных PostgreSQL.
    void initialize() override;

    /// \~english @brief Finalizes the connection to the PostgreSQL database.
    /// \~russian @brief Завершает соединение с базой данных PostgreSQL.
    void finalize() override;

    /// \~english @brief Sends a query to the PostgreSQL database.
    /// \~russian @brief Отправляет запрос к базе данных PostgreSQL.
    /// \~english @param query The SQL query to execute.
    /// \~russian @param query SQL-запрос для выполнения.
    /// \~english @return The result of the query as a tree of nodes.
    /// \~russian @return Результат запроса в виде дерева узлов.
    virtual INode::TSharedPtr
        sendQuery(std::string const &query) override;

private:
    /// \~english @brief The PostgreSQL connection handle.
    /// \~russian @brief Указатель на соединение PostgreSQL.
    PGconn* m_connection;
};


} // implementation
} // db
} // iridium


#endif // BUILD_FLAG_POSTGRES


#endif // HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C
