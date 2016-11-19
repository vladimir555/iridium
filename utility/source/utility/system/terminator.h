#include "utility/smart_ptr.h"
#include "utility/pattern/singleton.h"


namespace utility {
namespace system {


class ITerminatorHandler {
public:
    DEFINE_SMART_PTR(ITerminatorHandler);
    ///
    virtual ~ITerminatorHandler() = default;
    ///
    virtual void handleTerminator() = 0;
};


class Terminator:
    public pattern::Singleton//,
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
    std::list<ITerminatorHandler::TWeakPtr> m_handlers;
};


} // system
} // utility
