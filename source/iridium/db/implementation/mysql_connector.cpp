#include "iridium/build_flags.h"


#ifdef BUILD_FLAG_MYSQL


#include "mysql_connector.h"

#include <iridium/convertion/convert.h>
#include <iridium/logging/logger.h>
#include <iridium/items.h>


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


CMySQLConnector::CMySQLConnector(config::TDatebase const &config)
:
    CConnector(config)
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
    auto result = mysql_real_connect(
       &m_connection, 
        m_config.Host.get().c_str(),
        m_config.User.get().c_str(),
        m_config.Password.get().c_str(),
        m_config.Database.get().empty() ? nullptr : m_config.Database.get().c_str(),
        m_config.Port.get() ? 3306 : m_config.Port.get(),
        nullptr, 0);

    if (!result) {
        mysql_close(&m_connection);
        throw Exception("connect to mysql host error: " + string(mysql_error(&m_connection))); // ----->
    }
    LOGI << "initialization mysql '" << m_config.Host.get() << "' database '" << m_config.Database.get() << "' done";
}


void CMySQLConnector::finalize() {
    mysql_close(&m_connection);
    LOGI << "finalization mysql '" << m_config.Host.get() << "' database '" << m_config.Database.get() << "' done";
}


CMySQLConnector::TRows CMySQLConnector::sendQuery(string const &query) {
    LOGT << "send sql query: " << query;
    TRows rows;
    if (mysql_query(&m_connection, query.c_str())) {
        throw Exception("failed to send query: " + string(mysql_error(&m_connection))); // ----->
    } else {
        if (mysql_warning_count(&m_connection))
            LOGW << "mysql warnings:\n" << displayWarnings(&m_connection);

        // query succeeded, process any data returned by it
        auto result         = mysql_store_result(&m_connection);
        auto fields         = mysql_fetch_fields(result);
        auto fields_size    = mysql_num_fields  (result);

        if (result) {
            // there are rows
            while (auto row = mysql_fetch_row(result)) {
                TRow r;
                for (unsigned int i = 0; i < fields_size; i++) {
                    switch (fields[i].type) {
                    case MYSQL_TYPE_BIT:
                        r[fields[i].name] = row[i] ? "1" : "0";
                        break;
                    default:
                        r[fields[i].name] = row[i] ? row[i] : "null";
                    }
                }

                rows.push_back(r);
            }
            mysql_free_result(result);
        } else {
            // mysql_store_result() returned nothing; should it have ?
            if (mysql_field_count(&m_connection) == 0) {
                // query does not return data
                // (it was not a SELECT)
                auto rows_size = mysql_affected_rows(&m_connection);
                TRow row;
                row[FIELD_NAME_AFFECTED_ROWS] = convert<string>(rows_size);
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
