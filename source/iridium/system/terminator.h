//// Copyright © 2019 Bulaev Vladimir.
//// Contacts: <bulaev_vladimir@mail.ru>
//// License: https://www.gnu.org/licenses/lgpl-3.0
//
//#ifndef HEADER_TERMINATOR_14A1C7DA_9ECF_49BD_8318_A7E5A92FDC8A
//#define HEADER_TERMINATOR_14A1C7DA_9ECF_49BD_8318_A7E5A92FDC8A
//
//
//#include "iridium/smart_ptr.h"
//#include "iridium/pattern/singleton.h"
//
//
//#include <list>
//
//
//namespace iridium {
//namespace system {
//
//
//class ITerminatorHandler {
//public:
//    DEFINE_SMART_PTR(ITerminatorHandler)
//    ///
//    virtual ~ITerminatorHandler() = default;
//public:
//    ///
//    virtual void handleTerminate() = 0;
//};
//
//
//class Terminator:
//    public pattern::Singleton<Terminator>//,
//    //public threading::implementation::CTimedMutex
//{
//public:
//    virtual ~Terminator() = default;
//    ///
//    void addHandler(ITerminatorHandler::TWeakPtr handler);
//
//private:
//    ///
//    friend class pattern::Singleton<Terminator>;
//    ///
//    friend void handleAppicationTerminate();
//    ///
//    Terminator();
//    ///
//    void handleTerminate();
//    ///
//    std::list<ITerminatorHandler::TWeakPtr> m_handlers;
//};
//
//
//} // system
//} // iridium
//
//
//#endif // HEADER_TERMINATOR_14A1C7DA_9ECF_49BD_8318_A7E5A92FDC8A
