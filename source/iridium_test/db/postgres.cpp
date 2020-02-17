/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <iridium/testing/tester.h>
#include <iridium/build_flags.h>


#ifdef BUILD_FLAG_POSTGRESQL


#include <iridium/db/implementation/postgres_connector.h>
#include <iridium/db/config.h>


using iridium::db::implementation::CPostgresConnector;
using iridium::io::net::URL;
using iridium::db::config::TDatebaseConnector;


namespace iridium {
namespace db {


TEST(postgres) {
    IConnector::TSharedPtr connector = CPostgresConnector::create(
        URL("postgres://localhost"), "postgres", "postgres", "postgres");
    connector->initialize();
    auto rows = connector->sendQuery("select * from pg_database;");
    connector->finalize();

    ASSERT(0, less, rows.size());
    less(0, rows.size(), "", "");
    for (auto const &row: rows) {
        LOGT << "";
        for (auto const &i: row)
            LOGT << i.first << " = " << i.second;
    }
}


} // db
} // iridium


#endif // BUILD_FLAG_POSTGRESQL
