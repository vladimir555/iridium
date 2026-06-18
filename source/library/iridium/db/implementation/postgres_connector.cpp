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


namespace iridium::db::implementation {


CPostgresConnector::CPostgresConnector(io::URI const &uri)
:
    CConnector  (uri),
    m_connection(nullptr)
{}


CPostgresConnector::~CPostgresConnector() {
}


void handlePostgresMessage(void *, PGresult const *result) {
    LOGD << string(PQresultErrorField(result, PG_DIAG_MESSAGE_PRIMARY));
}


void CPostgresConnector::initialize() {
//    m_connection = std::make_shared<pqxx::connection>(
//         "host="        + convert<string>(m_uri)    +
//        " user="        + m_user                    +
//        " password="    + m_password                +
//        " dbname="      + m_database);
//    if (!result) {
//        // ...
//        throw DBException("connect to mysql host error: " + e.what())); // ----->
//    }

    auto path = m_uri.getPath();
    if (!path.empty())
        path = path.substr(1);

    m_connection = assertExists(
        PQconnectdb(
            string(
                "host='"        + m_uri.getHost()
            + "' port='"        + convert<std::string>(m_uri.getPort())
            + "' user='"        + m_uri.getUser()
            + "' password='"    + m_uri.getPassword()
            + "' dbname='"      + path + "'").c_str()),
                "connection to postgresql host error: null connector");

    if (PQstatus(m_connection) != CONNECTION_OK) {
        string error = PQerrorMessage(m_connection);
        PQfinish(m_connection);
        m_connection = nullptr;
        throw Exception(
            "connection to postgresql '" + convert<string>(m_uri) + "' error: " + error); // ----->
    }

    PQsetNoticeReceiver(m_connection, handlePostgresMessage, nullptr);

    LOGI << "initialization postgres '" << m_uri << "' done";
}


void CPostgresConnector::finalize() {
    if (m_connection) {
        PQfinish(m_connection);
        m_connection = nullptr;
        LOGI << "finalization postgres '" << m_uri << "' done";
    }
}


CPostgresConnector::INode::TSharedPtr CPostgresConnector::sendQuery(string const &query) {
    LOGD << "send postgres sql query:\n" << query;
//    TRows rows;
    auto table  = CNode::create(m_uri.getPath());
    auto result = PQexec(m_connection, query.c_str());
    auto status = PQresultStatus(result);

    if (status == PGRES_TUPLES_OK) {
        for (int row_index = 0; row_index < PQntuples(result); row_index++) {
//            TRow row;
            for (int field_index = 0; field_index < PQnfields(result); field_index++)
                table->addChild(PQfname(result, field_index), PQgetvalue(result, row_index, field_index));
//                row[PQfname(result, field_index)] = PQgetvalue(result, row_index, field_index);
//            rows.push_back(row);
        }
    }

    else

    if (checkOneOf(status, PGRES_COMMAND_OK, PGRES_EMPTY_QUERY)) {
        return {};
    } else {
        string error = PQerrorMessage(m_connection);
        PQclear(result);
        //PQfinish(m_connection);
        throw Exception("query to postgresql host error: " + error); // ----->
    }

    PQclear(result);

    return table;
//    return rows; // ----->
}


} // iridium::db::implementation


#endif // BUILD_FLAG_POSTGRES
