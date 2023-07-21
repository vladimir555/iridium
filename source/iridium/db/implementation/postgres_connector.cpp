#include "iridium/build_flags.h"


#ifdef BUILD_FLAG_POSTGRES


#include "postgres_connector.h"

#include <iridium/convertion/convert.h>
#include <iridium/logging/logger.h>
#include <iridium/assert.h>


using std::string;
using std::vector;
using iridium::io::URI;
using iridium::convertion::convert;
using iridium::convertion::convertPtr;


namespace iridium {
namespace db {
namespace implementation {


CPostgresConnector::CPostgresConnector(config::TDatebase const &config)
:
    CConnector  (config),
    m_connection(nullptr)
{}


CPostgresConnector::~CPostgresConnector() {
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
    m_connection = assertExists(PQconnectdb(string(
        "host='"        + m_config.Host.get()       + "'" +
       " port='"        + convert<std::string>(m_config.Port.get()) + "'" +
       " user='"        + m_config.User.get()       + "'" +
       " password='"    + m_config.Password.get()   + "'" +
       " dbname='"      + m_config.Database.get()   + "'").c_str()),
        "connect to postgresql host error: null connector");

    if (PQstatus(m_connection) != CONNECTION_OK) {
        string error = PQerrorMessage(m_connection);
        PQfinish(m_connection);
        throw Exception("connect to postgresql '" + m_config.Host.get() + "' error: " + error); // ----->
    }

    LOGI << "initialization postgres '" << m_config.Host.get() << "' database '" << m_config.Database.get() << "' done";
}


void CPostgresConnector::finalize() {
    PQfinish(m_connection);
}


void CPostgresConnector::executeCommand(std::string const &command) {

}


CPostgresConnector::TRows CPostgresConnector::sendQuery(string const &query) {
//    LOGT << "send sql query: " << query;
    TRows rows;

    auto result = PQexec(m_connection, query.c_str());
//    LOGT << "status: " << int(PQresultStatus(result));
    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        for (int row_index = 0; row_index < PQntuples(result); row_index++) {
            TRow row;
            for (int field_index = 0; field_index < PQnfields(result); field_index++)
                row[PQfname(result, field_index)] = PQgetvalue(result, row_index, field_index);
            rows.push_back(row);
        }
    } else {
        string error = PQerrorMessage(m_connection);
        PQclear(result);
        PQfinish(m_connection);
        throw Exception("query to postgresql host error: " + error); // ----->
    }
    PQclear(result);


//    pqxx::work      action(*assertExists(m_connection, "postgres not connected"), "postgres action");
//    pqxx::result    result = action.exec(query);
//    for (auto const &line: result) {
//        TRow row;
//        for (auto const field: line)
//            row[field.name()] = field.c_str();
//        rows.push_back(row);
//    }

    return rows; // ----->
}


} // implementation
} // db
} // iridium


#endif // BUILD_FLAG_POSTGRES
