/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "worker.h"

#include "iridium/logging/logger.h"


namespace iridium {
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
} // iridium
