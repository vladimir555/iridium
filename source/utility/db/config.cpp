#include "config.h"

#include "implementation/mysql_connector.h"

#include "utility/build_flags.h"


using utility::db::config::TDatebaseConnector;
using utility::convertion::convert;
using std::string;


#ifdef BUILD_FLAG_MYSQL
using utility::db::implementation::CMySQLConnector;
#endif // BUILD_FLAG_MYSQL


namespace utility {
namespace db {


IDBConnector::TSharedPtr createConnector(TDatebaseConnector const &config) {
    switch (static_cast<IDBConnector::TDBType>(config.Type)) {
    case IDBConnector::TDBType::UNKNOWN:
        throw std::runtime_error("wrong config: unknown db type"); // ----->
#ifdef BUILD_FLAG_MYSQL
    case IDBConnector::TDBType::MYSQL:
        return CMySQLConnector::create(config.Url, config.User, config.Password, config.Database); // ----->
#endif // BUILD_FLAG_MYSQL
    default:
        throw std::runtime_error("wrong config: unknown db type " + convert<string>(config.Type)); // ----->
    }
}


} // db
} // utility
