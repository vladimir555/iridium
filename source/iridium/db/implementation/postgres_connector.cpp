// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Implements the `CPostgresConnector` class.
/// \~russian @file
/// @brief Реализует класс `CPostgresConnector`.


#include "iridium/build_flags.h"


#ifdef BUILD_FLAG_POSTGRES


#include "postgres_connector.h"


#include "iridium/convertion/convert.h"
#include "iridium/logging/logger.h"
#include "iridium/assert.h"
#include "iridium/items.h"
#include "iridium/parsing/implementation/node.h"


using std::string;
using std::vector;
using iridium::io::URI;
using iridium::convertion::convert;
using iridium::parsing::implementation::CNode;


namespace {


/// \~english @brief Handles notice messages from the PostgreSQL server.
/// \~russian @brief Обрабатывает уведомления от сервера PostgreSQL.
void handlePostgresMessage(void *, PGresult const *result) {
    LOGD << PQresultErrorField(result, PG_DIAG_MESSAGE_PRIMARY);
}


} // unnamed


namespace iridium::db::implementation {


CPostgresConnector::CPostgresConnector(config::TDatebase const &config)
:
    CConnector  (config),
    m_connection(nullptr)
{}


CPostgresConnector::~CPostgresConnector() {}


void CPostgresConnector::initialize() {
    m_connection = assertExists(PQconnectdb(string(
        "host='"        + m_config.Host.get()       + "'" +
       " port='"        + convert<std::string>(m_config.Port.get()) + "'" +
       " user='"        + m_config.User.get()       + "'" +
       " password='"    + m_config.Password.get()   + "'" +
       " dbname='"      + m_config.Database.get()   + "'").c_str()),
        "connection to postgresql host error: null connector");

    if (PQstatus(m_connection) != CONNECTION_OK) {
        string error = PQerrorMessage(m_connection);
        PQfinish(m_connection);
        m_connection = nullptr;
        throw Exception("connection to postgresql '" + m_config.Host.get() + "' error: " + error); // ----->
    }

    PQsetNoticeReceiver(m_connection, handlePostgresMessage, nullptr);

    LOGI << "initialization postgres '" << m_config.Host.get() << "' database '" << m_config.Database.get() << "' done";
}


void CPostgresConnector::finalize() {
    if (m_connection) {
        PQfinish(m_connection);
        m_connection = nullptr;
        LOGI << "finalization postgres '" << m_config.Host.get() << "' database '" << m_config.Database.get() << "' done";
    }
}


CPostgresConnector::INode::TSharedPtr CPostgresConnector::sendQuery(string const &query) {
    LOGD << "send postgres sql query:\n" << query;
    auto table  = CNode::create(m_config.Database.get());
    auto result = PQexec(m_connection, query.c_str());
    auto status = PQresultStatus(result);

    if (status == PGRES_TUPLES_OK) {
        for (int row_index = 0; row_index < PQntuples(result); row_index++) {
            for (int field_index = 0; field_index < PQnfields(result); field_index++)
                table->addChild(PQfname(result, field_index), PQgetvalue(result, row_index, field_index));
        }
    }

    else

    if (checkOneOf(status, PGRES_COMMAND_OK, PGRES_EMPTY_QUERY)) {
        return {};
    } else {
        string error = PQerrorMessage(m_connection);
        PQclear(result);
        throw Exception("query to postgresql host error: " + error); // ----->
    }

    PQclear(result);

    return table;
}


} // iridium::db::implementation


#endif // BUILD_FLAG_POSTGRES
