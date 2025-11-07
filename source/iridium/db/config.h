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


/**
 * @brief Defines the structure for database configuration.
 *
 * This structure is designed to be serialized from/to a configuration file (e.g., JSON).
 * It holds all the necessary parameters to establish a database connection.
 */
DEFINE_ROOT_NODE_BEGIN(Datebase)
    /**
     * @brief Enumeration for the supported database types.
     */
    DEFINE_ENUM(
        TDBType,
        MYSQL,    ///< MySQL database.
        POSTGRES  ///< PostgreSQL database.
    );

    /**
     * @brief The type of the database (e.g., MYSQL, POSTGRES).
     */
    DEFINE_ATTRIBUTE(TDBType, Type, {})

    /**
     * @brief The hostname or IP address of the database server.
     */
    DEFINE_ATTRIBUTE(std::string, Host, {})

    /**
     * @brief The port number of the database server.
     */
    DEFINE_ATTRIBUTE(uint16_t, Port, {})

    /**
     * @brief The username for authentication.
     */
    DEFINE_ATTRIBUTE(std::string, User, {})

    /**
     * @brief The password for authentication.
     */
    DEFINE_ATTRIBUTE(std::string, Password, {})

    /**
     * @brief The name of the database to connect to.
     */
    DEFINE_ATTRIBUTE(std::string, Database, {})
DEFINE_ROOT_NODE_END()


} // config


} // db
} // iridium


//DEFINE_ENUM_CONVERT(iridium::db::config::TDatebase::TDBType)


#endif // HEADER_CONFIG_08F54573_BC06_4817_9AE5_35A3BC49E892
