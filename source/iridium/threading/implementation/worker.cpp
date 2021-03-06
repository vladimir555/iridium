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


void CWorkerHandler::handle(TItems const &items_) {
    for (auto const &item: items_)
        item->execute();
}


} // implementation
} // threading
} // iridium
