#include <iridium/testing/tester.h>
#include <iridium/build_flags.h>


#ifdef BUILD_FLAG_MYSQL


#include <iridium/db/factory.h>


namespace iridium {
namespace db {


TEST(mysql) {
    auto connector = createConnector(io::URI("mysql://root:root@localhost/information_schema"));
    connector->initialize();
    auto rows = connector->sendQuery("select * from ENGINES;");
    connector->finalize();

    //ASSERT(0, less, rows.size());
    //for (auto const &row: rows) {
    //    LOGT << "";
    //    for (auto const &i: row)
    //        LOGT << i.first << " = " << i.second;
    //}
}


} // db
} // iridium


#endif // BUILD_FLAG_POSTGRES
