#include "iridium/build_flags.h"


#ifdef BUILD_FLAG_MYSQL


#include "mysql_connector.h"

#include <iridium/convertion/convert.h>
#include <iridium/logging/logger.h>


using std::string;
using std::vector;
using iridium::io::URI;
using iridium::convertion::convert;


namespace {


string displayWarnings(MYSQL *connection) {
    MYSQL_RES *sql_result;
    MYSQL_ROW row;
    if ((connection == NULL) || mysql_query(connection, "SHOW WARNINGS")) {
        return "Can not display list of errors/warnings!";
    }

    sql_result = mysql_store_result(connection);
    if ((sql_result == NULL) || (sql_result->row_count == 0)) {
        if (sql_result)
            mysql_free_result(sql_result);
        return "Can not display list of errors/warnings!";
    }

    row = mysql_fetch_row(sql_result);
    if (row == NULL) {
        mysql_free_result(sql_result);
        return "Can not display list of errors/warnings!";
    }

    string result;
    do {
        // Format: "message [Type: number]"
        result += static_cast<char const * const>(row[2]) + string("[") + static_cast<char const * const>(row[0]) + ":" + static_cast<char const * const>(row[1]) + "]\n";
        row = mysql_fetch_row(sql_result);
    } while (row);
    mysql_free_result(sql_result);

    return result;
}


} // unnamed


namespace iridium {
namespace db {
namespace implementation {


CMySQLConnector::CMySQLConnector(URI const &uri, string const &user, string const &password, string const &database)
:
    m_uri       (uri),
    m_user      (user),
    m_password  (password),
    m_database  (database)
{
    mysql_library_init(0, nullptr, nullptr);
    mysql_init(&m_connection);
    static bool const is_reconnect = true;
    mysql_options(&m_connection, MYSQL_OPT_RECONNECT, &is_reconnect);
}


CMySQLConnector::~CMySQLConnector() {
    mysql_library_end();
}


void CMySQLConnector::initialize() {
    string host = m_uri.getHost();

    if (host.empty())
        host = m_uri.getAddress();

    auto port = m_uri.getPort();

    auto result = mysql_real_connect(
       &m_connection, 
        host.c_str(), 
        m_user.c_str(), 
        m_password.c_str(), 
        m_database.empty() ? nullptr : m_database.c_str(), 
        port, 
        nullptr, 0);

    if (!result) {
        mysql_close(&m_connection);
        throw Exception("connect to mysql host error: " + string(mysql_error(&m_connection))); // ----->
    }
    LOGI << "initialization MYSQL '" << m_uri << "' database '" << m_database << "' done";
}


void CMySQLConnector::finalize() {
    mysql_close(&m_connection);
}


CMySQLConnector::TRows CMySQLConnector::sendQuery(string const &query) {
    LOGD << "send sql query: " << query;
    TRows rows;
    if (mysql_query(&m_connection, query.c_str())) {
        throw Exception("failed to send query: " + string(mysql_error(&m_connection))); // ----->
    } else {
        if (mysql_warning_count(&m_connection))
            LOGW << "mysql warnings:\n" << displayWarnings(&m_connection);

        // query succeeded, process any data returned by it
        auto result = mysql_store_result(&m_connection);
        if (result) {
            // there are rows
            auto field_count    = mysql_num_fields(result);
            auto fields_        = mysql_fetch_fields(result);

            vector<string> fields;
            for (unsigned int i = 0; i < field_count; i++)
                fields.push_back(fields_[i].name);

            auto row_ = mysql_fetch_row(result);
            while (row_) {
                TRow row;
                for (unsigned int i = 0; i < field_count; i++)
                    row[fields[i]] = row_[i];
                rows.push_back(row);
                row_ = mysql_fetch_row(result);
            }
            mysql_free_result(result);
        } else {
            // mysql_store_result() returned nothing; should it have ?
            if (mysql_field_count(&m_connection) == 0) {
                // query does not return data
                // (it was not a SELECT)
                auto row_count = mysql_affected_rows(&m_connection);
                TRow row;
                row[FIELD_NAME_AFFECTED_ROWS] = convert<string>(row_count);
                rows.push_back(row);
            } else {
                // mysql_store_result() should have returned data
                throw Exception("failed to send query: " + string(mysql_error(&m_connection))); // ----->
            }
        }
    }

    return rows; // ----->
}


} // implementation
} // db
} // iridium


#endif // COMPILATION_FLAG_MYSQL
