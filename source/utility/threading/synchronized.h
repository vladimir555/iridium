#ifndef HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
#define HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD


#include "mutex.h"


namespace utility {
namespace threading {


class Synchronized {
public:
    Synchronized(IMutex::TSharedPtr const &mutex);
    virtual ~Synchronized() = default;

    IMutex::TSharedPtr getMutex() const;

private:
    IMutex::TSharedPtr m_mutex;
};


} // threading
} // utility


#endif // HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
