#ifndef HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155
#define HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155


#include "utility/build_flags.h"
#ifdef BUILD_FLAG_MYSQL


#include "utility/smart_ptr.h"
#include "utility/db/connector.h"
#include "utility/net/url.h"

#include "connector.h"

#include <string>
#include <mysql/mysql.h>


namespace utility {
namespace db {
namespace implementation {


class CMySQLConnector: 
    public IDBConnector,
    public CConnector
{
public:
    DEFINE_CREATE(CMySQLConnector)
    ///
    CMySQLConnector(net::URL const &url, std::string const &user, std::string const &password, std::string const &database = "");
    ///
    virtual        ~CMySQLConnector();
    ///
    virtual void    initialize() override;
    ///
    virtual void    finalize() override;
    ///
    virtual TRows   sendQuery(std::string const &query) override;

private:
    ///
    ::MYSQL         m_connection;
    ///
    net::URL m_url;
    ///
    std::string     m_user;
    ///
    std::string     m_password;
    ///
    std::string     m_database;
};


} // implementation
} // db
} // utility


#endif // COMPILATION_FLAG_MYSQL


#endif // HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155
