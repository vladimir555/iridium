/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "terminator.h"


namespace iridium {
namespace system {


void handleAppicationTerminate() {
    Terminator::instance().handleTerminate();
}


void Terminator::addHandler(ITerminatorHandler::TWeakPtr handler) {
    m_handlers.push_back(handler);
}


Terminator::Terminator() {
    std::set_terminate(&handleAppicationTerminate);
}


void Terminator::handleTerminate() {
    //LOCK_SCOPE
    try {
        // todo: broadcast
        for (auto handler: m_handlers)
            if (!handler.expired())
                handler.lock()->handleTerminate();
    } catch (std::exception &) {
        // todo:
    }
}


} // system
} // iridium
