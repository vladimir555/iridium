// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D
#define HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D


#include "iridium/dp/non_copyable.h"
#include "iridium/io/uri.h"


namespace iridium::db::implementation {


class CConnector: public dp::NonCopyable {
public:
    CConnector(io::URI const &uri);
    virtual ~CConnector() = default;

protected:
    io::URI m_uri;
};


} // iridium::db::implementation


#endif // HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D
