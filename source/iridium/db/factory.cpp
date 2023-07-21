#include "factory.h"


#include "implementation/mysql_connector.h"
#include "implementation/postgres_connector.h"

#include "iridium/build_flags.h"


using iridium::db::config::TDatebase;
using iridium::io::URI;
using iridium::convertion::convert;
using std::string;


#ifdef BUILD_FLAG_MYSQL
using iridium::db::implementation::CMySQLConnector;
#endif // BUILD_FLAG_MYSQL
#ifdef BUILD_FLAG_POSTGRES
using iridium::db::implementation::CPostgresConnector;
#endif // BUILD_FLAG_POSTGRES


#include "iridium/logging/logger.h"
namespace iridium {
namespace db {


IConnector::TSharedPtr createConnector(TDatebase const &config) {
    switch (config.Type.get()) {
#ifdef BUILD_FLAG_MYSQL
    case TDatebase::TDBType::MYSQL:
        return CMySQLConnector::create(config); // ----->
#endif // BUILD_FLAG_MYSQL
#ifdef BUILD_FLAG_POSTGRES
    case TDatebase::TDBType::POSTGRES:
        return CPostgresConnector::create(config); // ----->
#endif // BUILD_FLAG_POSTGRES
    default:
        throw std::runtime_error("creating db connector error: unknown db type " + 
            convert<string>(config.Type.get())); // ----->
    }
}


IConnector::TSharedPtr createConnector(io::URI const &uri) {
    TDatebase config;

    switch (uri.getProtocol()) {
    case URI::TProtocol::POSTGRES:
        config.Type = TDatebase::TDBType::POSTGRES;
        break;
    case URI::TProtocol::MYSQL:
        config.Type = TDatebase::TDBType::MYSQL;
        break;
    default:
        throw std::runtime_error("creating db connector error: unknown db type " +
            convert<string>(uri.getProtocol())); // ----->
    }

    config.User     = uri.getUser();
    config.Password = uri.getPassword();
    config.Host     = uri.getHost();
    config.Port     = uri.getPort();
    config.Database = uri.getPath();

    LOGT << "db config:\n" << config.getNode();

    return createConnector(config); // ----->
}


} // db
} // iridium
