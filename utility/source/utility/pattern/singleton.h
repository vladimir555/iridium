



#include "non_copyable.h"
#include "non_movable.h"


namespace utility {
namespace pattern {


///
/// \brief The Singleton class
///
template<typename TClass>
class Singleton:
    public NonCopyable,
    public NonMovable
{
public:
    virtual ~Singleton() = default;
    ///
    static TClass &instance();
};


template<typename TClass>
TClass &Singleton<TClass>::instance() {
    static TClass m_instance;
    return m_instance; // ----->
}


} // pattern
} // utility
