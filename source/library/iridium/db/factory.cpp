// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "factory.h"


#include "implementation/mysql_connector.h"
#include "implementation/postgres_connector.h"

#include "iridium/build_flags.h"


using iridium::io::URI;
using iridium::convertion::convert;
using std::string;


#ifdef BUILD_FLAG_MYSQL
using iridium::db::implementation::CMySQLConnector;
#endif // BUILD_FLAG_MYSQL
#ifdef BUILD_FLAG_POSTGRES
using iridium::db::implementation::CPostgresConnector;
#endif // BUILD_FLAG_POSTGRES


namespace iridium::db {


IConnector::TSharedPtr createConnector(io::URI const &uri) {
    switch (uri.getProtocol()) {
#ifdef BUILD_FLAG_MYSQL
    case URI::TProtocol::MYSQL:
        return CMySQLConnector::create(uri); // ----->
#endif // BUILD_FLAG_MYSQL
#ifdef BUILD_FLAG_POSTGRES
    case URI::TProtocol::POSTGRES:
        return CPostgresConnector::create(uri); // ----->
#endif // BUILD_FLAG_POSTGRES
    case URI::TProtocol::UNKNOWN:
    default:
        throw std::runtime_error("creating db connector error: unknown db type " +
            convert<string>(uri.getProtocol())); // ----->
    }
}


} // namespace iridium::db
