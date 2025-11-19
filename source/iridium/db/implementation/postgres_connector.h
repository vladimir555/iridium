// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C
#define HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C


#include "iridium/build_flags.h"
#ifdef BUILD_FLAG_POSTGRES


#include "iridium/smart_ptr.h"
#include "iridium/db/connector.h"
#include "iridium/io/uri.h"

#include "connector.h"

#include <string>
#include <libpq-fe.h>


namespace iridium::db::implementation {


class CPostgresConnector :
    public IConnector,
    public CConnector
{
public:
    DEFINE_CREATE(CPostgresConnector)
    CPostgresConnector(config::TDatebase const &config);
    virtual ~CPostgresConnector() override;
    void initialize() override;
    void finalize() override;
    virtual INode::TSharedPtr sendQuery(std::string const &query) override;

private:
    PGconn *m_connection;
};


} // iridium::db::implementation


#endif // BUILD_FLAG_POSTGRES


#endif // HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C
