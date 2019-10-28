#ifndef HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
#define HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C


#include "iridium/enum.h"
#include "iridium/smart_ptr.h"
#include "iridium/pattern/initializable.h"

#include <string>
#include <vector>
#include <map>


namespace iridium {
namespace db {


class DBException : public std::runtime_error {
public:
    explicit DBException(std::string const &e);
    virtual ~DBException() = default;
};


class IDBConnector: public pattern::IInitializable {
public:
    DEFINE_ENUM(
        TDBType,
        MYSQL,
        POSTGRESQL
    )

    DEFINE_SMART_PTR(IDBConnector)
    virtual ~IDBConnector() = default;
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
