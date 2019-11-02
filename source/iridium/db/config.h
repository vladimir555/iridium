// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892
#define HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892


#include <string>
#include "iridium/io/net/url.h"
#include "iridium/parsing/serialization/node.h"
#include "iridium/db/connector.h"


namespace iridium {
namespace db {
namespace config {


DEFINE_ROOT_NODE_BEGIN(DatebaseConnector)
    DEFINE_ATTRIBUTE(io::net::URL           , Url)
    DEFINE_ATTRIBUTE(IDBConnector::TDBType  , Type)
    DEFINE_ATTRIBUTE(std::string            , User)
    DEFINE_ATTRIBUTE(std::string            , Password)
    DEFINE_ATTRIBUTE_DEFAULT(std::string    , Database, "")
DEFINE_ROOT_NODE_END()


} // config


/// IDBConnector builder
IDBConnector::TSharedPtr createConnector(config::TDatebaseConnector const &config);


} // db
} // iridium


#endif // HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892
