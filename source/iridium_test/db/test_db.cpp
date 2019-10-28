#include <gtest/gtest.h>

#include <iridium/build_flags.h>


#ifdef BUILD_FLAG_POSTGRESQL


#include <iridium/db/implementation/postgresql_connector.h>


namespace iridium {
namespace db {


TEST(db, postgres) {
    IDBConnector::TSharedPtr connector = implementation::CPostgreSQLConnector::create(net::URL("postgresql://localhost"), "volodja", "", "postgres");
    connector->initialize();
    auto rows = connector->sendQuery("select * from pg_database;");
    connector->finalize();

    ASSERT_LE(0, rows.size());
//    for (auto const &row: rows) {
//        LOGT << "";
//        for (auto const &i: row)
//            LOGT << i.first << " = " << i.second;
//    }

//    sleep(1);
}


} // db
} // iridium


#endif // BUILD_FLAG_POSTGRESQL
