//#include "terminator.h"
//
//
//// todo: use signals
//
//
//namespace iridium {
//namespace system {
//
//
//void handleAppicationTerminate() {
//    Terminator::instance().handleTerminate();
//}
//
//
//void Terminator::addHandler(ITerminatorHandler::TWeakPtr handler) {
//    m_handlers.push_back(handler);
//}
//
//
//Terminator::Terminator() {
//    std::set_terminate(&handleAppicationTerminate);
//}
//
//
//void Terminator::handleTerminate() {
//    //LOCK_SCOPE
//    try {
//        // todo: broadcast
//        for (auto handler: m_handlers)
//            if (!handler.expired())
//                handler.lock()->handleTerminate();
//    } catch (std::exception &) {
//        // todo:
//    }
//}
//
//
//} // system
//} // iridium
