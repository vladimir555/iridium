#ifndef HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D
#define HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D


#include "utility/pattern/non_copyable.h"


namespace utility {
namespace db {
namespace implementation {


class CConnector : public pattern::NonCopyable {
public:
    CConnector() = default;
    virtual ~CConnector() = default;
};


} // implementation
} // db
} // utility


#endif // HEADER_CONNECTOR_926752A7_A77B_4CD8_9A9E_699A4945401D
