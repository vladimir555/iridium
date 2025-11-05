// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892
#define HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892


#include "iridium/parsing/serialization/node.h"
#include "iridium/enum.h"


namespace iridium {
namespace db {
namespace config {


DEFINE_ROOT_NODE_BEGIN(Datebase)
    DEFINE_ENUM(
        TDBType,
        MYSQL,
        POSTGRES
    );
    DEFINE_ATTRIBUTE(TDBType    , Type, {})
    DEFINE_ATTRIBUTE(std::string, Host, {})
    DEFINE_ATTRIBUTE(uint16_t   , Port, {})
    DEFINE_ATTRIBUTE(std::string, User, {})
    DEFINE_ATTRIBUTE(std::string, Password, {})
    DEFINE_ATTRIBUTE(std::string, Database, {})
DEFINE_ROOT_NODE_END()


} // config


} // db
} // iridium


//DEFINE_ENUM_CONVERT(iridium::db::config::TDatebase::TDBType)


#endif // HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892
