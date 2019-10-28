#ifndef HEADER_SYNCHRONIZED_63BFE515_3627_47E2_A357_FE7D548A4136
#define HEADER_SYNCHRONIZED_63BFE515_3627_47E2_A357_FE7D548A4136


#include "mutex.h"

#include <thread>
#include <string>
#include <map>

#include "iridium/pattern/non_copyable.h"
#include "iridium/pattern/non_movable.h"

#include "synchronized.h"


namespace iridium {
namespace threading {


class SynchronizedScope: 
    public pattern::NonCopyable,
    public pattern::NonMovable
{
public:
    explicit SynchronizedScope(Synchronized const *synchronized, std::string const &scope_name);
    ~SynchronizedScope();

private:
    IMutex::TSharedPtr m_mutex;
};


class SynchronizedScopeSimple: 
    public pattern::NonCopyable, 
    public pattern::NonMovable 
{
public:
    explicit SynchronizedScopeSimple(Synchronized const *synchronized);
    ~SynchronizedScopeSimple();
private:
    IMutex::TSharedPtr m_mutex;
};


} // threading
} // iridium


// TODO: store two mutex names for deadlock exception


#define LOCK_SCOPE \
iridium::threading::SynchronizedScope _(this, __FUNCTION__);


#define LOCK_SCOPE_FAST \
iridium::threading::SynchronizedScopeSimple _(this);


#endif // HEADER_SYNCHRONIZED_63BFE515_3627_47E2_A357_FE7D548A4136
