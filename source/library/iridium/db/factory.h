// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#pragma once


#include "connector.h"
#include "iridium/io/uri.h"


namespace iridium::db {


IConnector::TSharedPtr createConnector(io::URI const &uri);


} // namespace iridium::db
