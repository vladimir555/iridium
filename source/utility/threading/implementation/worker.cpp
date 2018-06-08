#include "worker.h"

#include "utility/logging/logger.h"


namespace utility {
namespace threading {
namespace implementation {


void CWorkerHandler::initialize() {}


void CWorkerHandler::finalize() {}


CWorkerHandler::TItems CWorkerHandler::handle(TItems const &items_) {
    TItems items;

    for (auto const &item: items_)
        if (!item->do_())
            items.push_back(item);

    return items; // ----->
}


} // implementation
} // threading
} // utility
