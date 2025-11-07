// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
#define HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C


#include "iridium/pattern/initializable.h"
#include "iridium/parsing/node.h"

#include <stdexcept>
#include <string>
#include <vector>


namespace iridium {
namespace db {


/**
 * @brief Represents an exception specific to database operations.
 */
class Exception : public std::runtime_error {
public:
    /**
     * @brief Construct a new Exception object.
     * @param e The error message.
     */
    explicit Exception(std::string const &e);
    virtual ~Exception() = default;
};

/**
 * @brief An interface for a database connector.
 *
 * This interface defines the contract for classes that provide a connection to a database.
 * It includes methods for sending queries and processing results.
 */
class IConnector: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IConnector)
    using INode = parsing::INode;

    /**
     * @brief Sends a query to the database.
     *
     * @param query The SQL query to be executed.
     * @return A shared pointer to the root INode of the result set.
     *         The result is expected to be a tree structure representing the query output.
     *         For example, a list of rows, where each row is a list of fields.
     */
    virtual INode::TSharedPtr sendQuery(std::string const &query) = 0;

    /**
     * @brief A helper function to extract field names from a query result.
     *
     * @param rows The root node of the query result, typically representing a list of rows.
     * @return A vector of strings containing the field names.
     */
    static std::vector<std::string> makeFields(parsing::INode::TSharedPtr const &rows);

    /**
     * @brief The name of the field that contains the number of affected rows
     *        for queries like INSERT, UPDATE, or DELETE.
     */
    static std::string const FIELD_NAME_AFFECTED_ROWS;
};


} // db
} // iridium


#endif // HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
