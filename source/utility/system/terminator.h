#ifndef HEADER_TERMINATOR_14A1C7DA_9ECF_49BD_8318_A7E5A92FDC8A
#define HEADER_TERMINATOR_14A1C7DA_9ECF_49BD_8318_A7E5A92FDC8A


#include "utility/smart_ptr.h"
#include "utility/pattern/singleton.h"


#include <list>


namespace utility {
namespace system {


class ITerminatorHandler {
public:
    DEFINE_SMART_PTR(ITerminatorHandler)
    ///
    virtual ~ITerminatorHandler() = default;
public:
    ///
    virtual void handleTerminate() = 0;
};


class Terminator:
    public pattern::Singleton<Terminator>//,
    //public threading::implementation::CTimedMutex
{
public:
    virtual ~Terminator() = default;
    ///
    void addHandler(ITerminatorHandler::TWeakPtr handler);

private:
    ///
    friend class pattern::Singleton<Terminator>;
    ///
    friend void handleAppicationTerminate();
    ///
    Terminator();
    ///
    void handleTerminate();
    ///
    std::list<ITerminatorHandler::TWeakPtr> m_handlers;
};


} // system
} // utility


#endif // HEADER_TERMINATOR_14A1C7DA_9ECF_49BD_8318_A7E5A92FDC8A
