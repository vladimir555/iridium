#include "connector.h"


namespace iridium::db::implementation {


CConnector::CConnector(io::URI const &uri)
:
    m_uri(uri)
{}


} // iridium::db::implementation
