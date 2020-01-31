/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <iridium/testing/tester.h>
#include <iridium/build_flags.h>


#ifdef BUILD_FLAG_POSTGRESQL


#include <iridium/db/implementation/postgresql_connector.h>


namespace iridium {
namespace db {


TEST(postgres) {
    IDBConnector::TSharedPtr connector = implementation::CPostgreSQLConnector::create(
        io::net::URL("postgresql://localhost"), "postgres", "", "postgres");
    connector->initialize();
    auto rows = connector->sendQuery("select * from pg_database;");
    connector->finalize();

//    ASSERT(0, less, rows.size());
    less(0, rows.size(), "", "");
    for (auto const &row: rows) {
        LOGT << "";
        for (auto const &i: row)
            LOGT << i.first << " = " << i.second;
    }

//    sleep(1);
}


} // db
} // iridium


#endif // BUILD_FLAG_POSTGRESQL
