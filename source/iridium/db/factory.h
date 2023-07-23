#pragma once


#include "config.h"
#include "connector.h"
#include "iridium/io/uri.h"


namespace iridium {
namespace db {


IConnector::TSharedPtr createConnector(config::TDatebase    const &config);
IConnector::TSharedPtr createConnector(io::URI              const &uri);


} // db
} // iridium
