// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
#define HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C


#include "iridium/pattern/initializable.h"

#include <stdexcept>
#include <string>
#include <vector>
#include <map>


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
    ///
    typedef std::map<std::string, std::string> TRow;
    ///
    typedef std::vector< TRow > TRows;
    ///
    virtual TRows sendQuery(std::string const &query) = 0;
    ///
    static std::vector<std::string> makeFields(TRows const &rows);
    ///
    static std::string const FIELD_NAME_AFFECTED_ROWS;
};


} // db
} // iridium


#endif // HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
