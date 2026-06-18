#pragma once


#include "connector.h"
#include "iridium/io/uri.h"


namespace iridium::db {


IConnector::TSharedPtr createConnector(io::URI const &uri);


} // namespace iridium::db
