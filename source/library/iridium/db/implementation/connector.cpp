// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "connector.h"


namespace iridium::db::implementation {


CConnector::CConnector(io::URI const &uri)
:
    m_uri(uri)
{}


} // iridium::db::implementation
