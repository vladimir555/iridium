#ifndef HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892
#define HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892


#include <string>
#include "utility/networking/url.h"
#include "utility/parsing/serialization/node.h"
#include "utility/db/connector.h"


namespace utility {
namespace db {
namespace config {


DEFINE_ROOT_NODE_BEGIN(DatebaseConnector)
    DEFINE_ATTRIBUTE(networking::URL        , Url)
    DEFINE_ATTRIBUTE(IDBConnector::TDBType  , Type)
    DEFINE_ATTRIBUTE(std::string            , User)
    DEFINE_ATTRIBUTE(std::string            , Password)
    DEFINE_ATTRIBUTE_DEFAULT(std::string    , Database, "")
DEFINE_ROOT_NODE_END()


} // config


/// IDBConnector builder
IDBConnector::TSharedPtr createConnector(config::TDatebaseConnector const &config);


} // db
} // utility


#endif // HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892
