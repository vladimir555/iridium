// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C
#define HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C


/// \~english @file
/// @brief Defines the `CPostgresConnector` class for connecting to a PostgreSQL database.
/// \~russian @file
/// @brief Определяет класс `CPostgresConnector` для подключения к базе данных PostgreSQL.


#include "iridium/build_flags.h"
#ifdef BUILD_FLAG_POSTGRES


#include "iridium/smart_ptr.h"
#include "iridium/db/connector.h"
#include "iridium/io/uri.h"

#include "connector.h"

#include <string>
#include <libpq-fe.h>


namespace iridium::db::implementation {


/// \~english @brief A concrete implementation of the `IConnector` interface for PostgreSQL.
/// \~russian @brief Конкретная реализация интерфейса `IConnector` для PostgreSQL.
class CPostgresConnector :
    public IConnector,
    public CConnector
{
public:
    DEFINE_CREATE(CPostgresConnector)

    /// \~english @brief Constructs a `CPostgresConnector` with the given database configuration.
    /// \~russian @brief Конструирует `CPostgresConnector` с заданной конфигурацией базы данных.
    CPostgresConnector(config::TDatebase const &config);

    /// \~english @brief Destroys the `CPostgresConnector` object.
    /// \~russian @brief Уничтожает объект `CPostgresConnector`.
    virtual ~CPostgresConnector() override;

    /// \~english @brief Initializes the connection to the database.
    /// \~russian @brief Инициализирует соединение с базой данных.
    void initialize() override;

    /// \~english @brief Finalizes the connection to the database.
    /// \~russian @brief Завершает соединение с базой данных.
    void finalize() override;

    /// \~english @brief Sends a query to the database.
    /// \~russian @brief Отправляет запрос к базе данных.
    virtual INode::TSharedPtr sendQuery(std::string const &query) override;

private:
    PGconn *m_connection;
};


} // iridium::db::implementation


#endif // BUILD_FLAG_POSTGRES


#endif // HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C
