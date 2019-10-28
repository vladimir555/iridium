#ifndef HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D
#define HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D


#include "iridium/pattern/non_copyable.h"


namespace iridium {
namespace db {
namespace implementation {


class CConnector : public pattern::NonCopyable {
public:
    CConnector() = default;
    virtual ~CConnector() = default;
};


} // implementation
} // db
} // iridium


#endif // HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D
