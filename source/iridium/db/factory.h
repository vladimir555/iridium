#pragma once


#include "config.h"
#include "connector.h"
#include "iridium/io/uri.h"


namespace iridium {
namespace db {


/**
 * @brief Creates a database configuration object from a URI.
 * @details Parses the given URI and extracts database connection parameters (like type, host, port, user, password, database name)
 *          to populate a `config::TDatebase` object.
 * @param uri The URI containing database connection information.
 *            The URI scheme might determine the database type (e.g., "mysql://", "postgres://").
 * @return A `config::TDatebase` object populated with parameters from the URI.
 * @throws iridium::db::Exception or other exceptions if URI parsing fails or scheme is unsupported.
 */
config::TDatebase makeConfig(io::URI const &uri);

/**
 * @brief Creates a database connector instance based on the provided configuration.
 * @details This function acts as a factory to instantiate a concrete `IConnector` implementation
 *          (e.g., a MySQL connector, PostgreSQL connector) suitable for the given configuration.
 * @param config The database configuration object (`config::TDatebase`) specifying connection parameters.
 *               The `Type` field in the config determines which type of connector is created.
 * @return A shared pointer (`IConnector::TSharedPtr`) to an `IConnector` instance.
 *         Returns a null pointer or throws an exception if a connector cannot be created for the given configuration.
 * @throws iridium::db::Exception or other exceptions on failure to create a connector.
 */
IConnector::TSharedPtr createConnector(config::TDatebase    const &config);

/**
 * @brief Creates a database connector instance from a URI.
 * @details This is a convenience function that first calls `makeConfig(uri)` to create a configuration
 *          object from the URI, and then calls `createConnector(config)` with that configuration.
 * @param uri The URI containing database connection information.
 * @return A shared pointer (`IConnector::TSharedPtr`) to an `IConnector` instance.
 *         Returns a null pointer or throws an exception if a connector cannot be created.
 * @throws iridium::db::Exception or other exceptions if URI parsing or connector creation fails.
 */
IConnector::TSharedPtr createConnector(io::URI              const &uri);


} // db
} // iridium
