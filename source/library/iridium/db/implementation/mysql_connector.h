// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155
#define HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155


#include "iridium/build_flags.h"
#ifdef IRIDIUM_BUILD_FLAG_MYSQL


#include "iridium/db/connector.h"
#include "iridium/io/uri.h"

#include "connector.h"

#include <string>
#include <mysql.h>


namespace iridium::db::implementation {


class CMySQLConnector:
    public IConnector,
    public CConnector
{
public:
    DEFINE_CREATE(CMySQLConnector)
    CMySQLConnector(io::URI const &uri);
    virtual ~CMySQLConnector();
    void initialize() override;
    void finalize() override;
    INode::TSharedPtr sendQuery(std::string const &query) override;

private:
    ::MYSQL m_connection;
};


} // iridium::db::implementation


#endif // IRIDIUM_BUILD_FLAG_MYSQL


#endif // HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155
