#include "config.h"

#include "implementation/mysql_connector.h"
#include "implementation/postgres_connector.h"
#include "implementation/mysql_connector.h"

#include "iridium/build_flags.h"


using iridium::db::config::TDatebaseConnector;
using iridium::convertion::convert;
using std::string;


IMPLEMENT_ENUM(iridium::db::config::TDatebaseConnector::TDBType)


//namespace {
//
//
//std::string convert_(iridium::db::config::TDatebaseConnector::TType const &value) {
//    return convert<std::string>(value); // ----->
//}
//
//
//}
//
//
//IMPLEMENT_CONVERT(std::string, iridium::db::config::TDatebaseConnector::TDBType, convert_)


#ifdef BUILD_FLAG_MYSQL
using iridium::db::implementation::CMySQLConnector;
#endif // BUILD_FLAG_MYSQL
#ifdef BUILD_FLAG_POSTGRES
using iridium::db::implementation::CPostgresConnector;
#endif // BUILD_FLAG_POSTGRES


namespace iridium {
namespace db {


IConnector::TSharedPtr createConnector(TDatebaseConnector const &config) {
    switch (config.Type.get()) {
    case TDatebaseConnector::TDBType::UNKNOWN:
        throw std::runtime_error("wrong config: unknown db type"); // ----->
#ifdef BUILD_FLAG_MYSQL
    case TDatebaseConnector::TDBType::TType::MYSQL:
        return CMySQLConnector::create(config.Url, config.User, config.Password, config.Database); // ----->
#endif // BUILD_FLAG_MYSQL
#ifdef BUILD_FLAG_POSTGRES
    case TDatebaseConnector::TDBType::POSTGRES:
        return  CPostgresConnector::create(config.Url, config.User, config.Password, config.Database); // ----->
#endif // BUILD_FLAG_POSTGRES
    default:
        throw std::runtime_error("wrong config: unknown db type " + convert<string>(config.Type)); // ----->
    }
}


} // db
} // iridium
