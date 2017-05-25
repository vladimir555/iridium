#include "runnuble.h"


namespace utility {
namespace threading {
namespace implementation {


CRunnuble::CRunnuble() 
:
    m_is_running(true)
{}


void CRunnuble::stop() {
    m_is_running = false;
    //todo: condition for waiting stop
}


} // implementation
} // threading
} // utility
