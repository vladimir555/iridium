#ifndef HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C
#define HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C


#include "iridium/build_flags.h"
#ifdef BUILD_FLAG_POSTGRESQL


#include "iridium/smart_ptr.h"
#include "iridium/db/connector.h"
#include "iridium/net/url.h"

#include "connector.h"

#include <string>
#include <libpq-fe.h>
//#include <pqxx/connection>


namespace iridium {
namespace db {
namespace implementation {


class CPostgreSQLConnector:
    public IDBConnector,
    public CConnector
{
public:
    DEFINE_CREATE(CPostgreSQLConnector)
    ///
    CPostgreSQLConnector(net::URL const &url, std::string const &user, std::string const &password, std::string const &database = "");
    ///
    virtual        ~CPostgreSQLConnector();
    ///
    virtual void    initialize() override;
    ///
    virtual void    finalize() override;
    ///
    virtual TRows   sendQuery(std::string const &query) override;

private:
    ///
    void            executeCommand(std::string const &command);
    ///
//    std::shared_ptr<pqxx::connection>   m_connection;
    PGconn             *m_connection;
    ///
    net::URL     m_url;
    ///
    std::string         m_user;
    ///
    std::string         m_password;
    ///
    std::string         m_database;
};


} // implementation
} // db
} // iridium


#endif // BUILD_FLAG_POSTGRESQL


#endif // HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C
