// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D
#define HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D


#include "iridium/pattern/non_copyable.h"
#include "iridium/db/config.h"


namespace iridium::db::implementation {


class CConnector : public pattern::NonCopyable {
public:
    CConnector(config::TDatebase const &config);
    virtual ~CConnector() = default;

protected:
    config::TDatebase m_config;
};


} // iridium::db::implementation


#endif // HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D
