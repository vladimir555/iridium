// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include <iridium/testing/tester.h>
#include <iridium/build_flags.h>


#ifdef IRIDIUM_BUILD_FLAG_POSTGRES


#include <iridium/db/factory.h>


namespace iridium::db {


TEST(postgres) {
    auto connector = createConnector(io::URI("postgres://postgres:postgres@localhost/postgres"));
    connector->initialize();
    auto rows = connector->sendQuery("select * from pg_database;");
    connector->finalize();

    ASSERT(0, less, rows->size());
    LOGT << rows;
//    for (auto const &row: rows) {
//        LOGT << "";
//        for (auto const &i: row)
//            LOGT << i.first << " = " << i.second;
//    }
}


} // namespace iridium::db


#endif // IRIDIUM_BUILD_FLAG_POSTGRES
