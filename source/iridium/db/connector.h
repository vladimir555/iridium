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


class Exception : public std::runtime_error {
public:
    explicit Exception(std::string const &e);
    virtual ~Exception() = default;
};


class IConnector: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IConnector)
    using INode = parsing::INode;
    ///
    virtual INode::TSharedPtr sendQuery(std::string const &query) = 0;
    ///
    static std::vector<std::string> makeFields(parsing::INode::TSharedPtr const &rows);
    ///
    static std::string const FIELD_NAME_AFFECTED_ROWS;
};


} // db
} // iridium


#endif // HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
