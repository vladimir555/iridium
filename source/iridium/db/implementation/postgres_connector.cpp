/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "iridium/build_flags.h"


#ifdef BUILD_FLAG_POSTGRES


#include "postgres_connector.h"

#include <iridium/convertion/convert.h>
#include <iridium/logging/logger.h>
#include <iridium/assert.h>


using std::string;
using std::vector;
using iridium::io::net::URL;
using iridium::convertion::convert;
using iridium::convertion::convertPtr;


namespace iridium {
namespace db {
namespace implementation {


CPostgresConnector::CPostgresConnector(URL const &url, string const &user, string const &password, string const &database)
:
    m_connection(nullptr),
    m_url       (url),
    m_user      (user),
    m_password  (password),
    m_database  (database)
{}


CPostgresConnector::~CPostgresConnector() {
}


void CPostgresConnector::initialize() {
//    m_connection = std::make_shared<pqxx::connection>(
//         "host="        + convert<string>(m_url)    +
//        " user="        + m_user                    +
//        " password="    + m_password                +
//        " dbname="      + m_database);
//    if (!result) {
//        // ...
//        throw DBException("connect to mysql host error: " + e.what())); // ----->
//    }
    m_connection = assertExists(PQconnectdb(string(
        "host='"        + convertPtr(m_url.getHost())   + "'" +
       " port='"        + convertPtr(m_url.getPort())   + "'" +
       " user='"        + m_user                        + "'" +
       " password='"    + m_password                    + "'" +
       " dbname='"      + m_database                    + "'").c_str()), "connect to postgresql host error: null connector");

    if (PQstatus(m_connection) != CONNECTION_OK) {
        string error = PQerrorMessage(m_connection);
        PQfinish(m_connection);
        throw Exception("connect to postgresql '" + convert<string>(m_url) + "' error: " + error); // ----->
    }

//    LOGI << "initialization Postgres '" << m_url << "' database '" << m_database << "' done";
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
