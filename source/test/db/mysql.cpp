// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include <iridium/testing/tester.h>
#include <iridium/build_flags.h>


#ifdef IRIDIUM_BUILD_FLAG_MYSQL


#include <iridium/db/factory.h>


namespace iridium::db {


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


} // namespace iridium::db


#endif // IRIDIUM_BUILD_FLAG_POSTGRES
