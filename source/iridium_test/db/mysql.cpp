#include <iridium/testing/tester.h>
#include <iridium/build_flags.h>


#ifdef BUILD_FLAG_MYSQL


#include <iridium/db/implementation/mysql_connector.h>
#include <iridium/db/config.h>


using iridium::db::implementation::CMySQLConnector;
using iridium::io::URI;
using iridium::db::config::TDatebaseConnector;


namespace iridium {
namespace db {


TEST(mysql) {
    IConnector::TSharedPtr connector = CMySQLConnector::create(
        URI("mysql://localhost"), "root", "1Tx6WyT@tgp", "information_schema");
    connector->initialize();
    auto rows = connector->sendQuery("select * from ENGINES;");
    connector->finalize();

    ASSERT(0, less, rows.size());
    //    less(0, rows.size(), "", "");
    //for (auto const &row: rows) {
    //    LOGT << "";
    //    for (auto const &i: row)
    //        LOGT << i.first << " = " << i.second;
    //}
}


} // db
} // iridium


#endif // BUILD_FLAG_POSTGRES
