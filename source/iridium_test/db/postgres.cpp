#include <iridium/testing/tester.h>
#include <iridium/build_flags.h>


#ifdef BUILD_FLAG_POSTGRES


#include <iridium/db/implementation/postgres_connector.h>
#include <iridium/db/config.h>


using iridium::db::implementation::CPostgresConnector;
using iridium::io::URI;
using iridium::db::config::TDatebaseConnector;


namespace iridium {
namespace db {


TEST(postgres) {
    IConnector::TSharedPtr connector = CPostgresConnector::create(
        URI("postgres://localhost"), "postgres", "postgres", "postgres");
    connector->initialize();
    auto rows = connector->sendQuery("select * from pg_database;");
    connector->finalize();

    ASSERT(0, less, rows.size());
//    less(0, rows.size(), "", "");
//    for (auto const &row: rows) {
//        LOGT << "";
//        for (auto const &i: row)
//            LOGT << i.first << " = " << i.second;
//    }
}


} // db
} // iridium


#endif // BUILD_FLAG_POSTGRES
