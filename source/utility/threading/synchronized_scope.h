#ifndef HEADER_SYNCHRONIZED_63BFE515_3627_47E2_A357_FE7D548A4136
#define HEADER_SYNCHRONIZED_63BFE515_3627_47E2_A357_FE7D548A4136


#include "mutex.h"

#include <thread>
#include <string>
#include <map>

#include "utility/pattern/non_copyable.h"
#include "utility/pattern/non_movable.h"


namespace utility {
namespace threading {


class SynchronizedScope: 
    public pattern::NonCopyable,
    public pattern::NonMovable
{
public:
    explicit SynchronizedScope(IMutex *mutex, std::string const &scope_name);
    virtual ~SynchronizedScope();

private:
    IMutex  *m_mutex;
};


class SynchronizedScopeSimple: 
    public pattern::NonCopyable, 
    public pattern::NonMovable 
{
public:
    explicit SynchronizedScopeSimple(IMutex *mutex);
   ~SynchronizedScopeSimple();
private:
    IMutex *m_mutex;
};


} // threading
} // utility


// TODO: store two mutex names for deadlock exception


#define LOCK_SCOPE \
utility::threading::SynchronizedScope _(this, __FUNCTION__);


#define LOCK_SCOPE_FAST \
utility::threading::SynchronizedScopeSimple _(this);


#endif // HEADER_SYNCHRONIZED_63BFE515_3627_47E2_A357_FE7D548A4136
