// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155
#define HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155


#include "iridium/build_flags.h"
#ifdef BUILD_FLAG_MYSQL


#include "iridium/smart_ptr.h"
#include "iridium/db/connector.h"
#include "iridium/net/uri.h"

#include "connector.h"

#include <string>
#include <mysql/mysql.h>


namespace iridium {
namespace db {
namespace implementation {


class CMySQLConnector: 
    public IDBConnector,
    public CConnector
{
public:
    DEFINE_CREATE(CMySQLConnector)
    ///
    CMySQLConnector(net::URI const &uri, std::string const &user, std::string const &password, std::string const &database = "");
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
    net::URI m_uri;
    ///
    std::string     m_user;
    ///
    std::string     m_password;
    ///
    std::string     m_database;
};


} // implementation
} // db
} // iridium


#endif // COMPILATION_FLAG_MYSQL


#endif // HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155
