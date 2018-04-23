#include "utility/build_flags.h"


#ifdef BUILD_FLAG_POSTGRESQL


#include "postgresql_connector.h"

#include <utility/convertion/convert.h>
#include <utility/logging/logger.h>
#include <utility/assert.h>
//#include <pqxx/transaction>



using std::string;
using std::vector;
using utility::networking::URL;
using utility::convertion::convert;
using utility::convertion::convertPtr;


namespace utility {
namespace db {
namespace implementation {


CPostgreSQLConnector::CPostgreSQLConnector(URL const &url, string const &user, string const &password, string const &database)
:
    m_connection(nullptr),
    m_url       (url),
    m_user      (user),
    m_password  (password),
    m_database  (database)
{}


CPostgreSQLConnector::~CPostgreSQLConnector() {
}


void CPostgreSQLConnector::initialize() {
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
        throw DBException("connect to postgresql '" + convert<string>(m_url) + "' error: " + error); // ----->
    }

    LOGI << "initialization Postgres '" << m_url << "' database '" << m_database << "' done";
}


void CPostgreSQLConnector::finalize() {
    PQfinish(m_connection);
}


void CPostgreSQLConnector::executeCommand(std::string const &command) {

}


CPostgreSQLConnector::TRows CPostgreSQLConnector::sendQuery(string const &query) {
    LOGT << "send sql query: " << query;
    TRows rows;

    auto result = PQexec(m_connection, query.c_str());
    LOGT << "status: " << int(PQresultStatus(result));
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
        throw DBException("query to postgresql host error: " + error); // ----->
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
} // utility


#else
#ifndef _MSC_VER
void dummy() {}
#endif
#endif // COMPILATION_FLAG_MYSQL
