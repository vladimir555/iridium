#include "config.h"

#include "implementation/mysql_connector.h"

#include "iridium/build_flags.h"


using iridium::db::config::TDatebaseConnector;
using iridium::convertion::convert;
using std::string;


namespace {


std::string convert_(iridium::db::config::TDatebaseConnector::TType const &v) {
    return convert<std::string>(v.get()); // ----->
}


}


IMPLEMENT_CONVERT(std::string, iridium::db::config::TDatebaseConnector::TType, convert_)


#ifdef BUILD_FLAG_MYSQL
using iridium::db::implementation::CMySQLConnector;
#endif // BUILD_FLAG_MYSQL


namespace iridium {
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
} // iridium
