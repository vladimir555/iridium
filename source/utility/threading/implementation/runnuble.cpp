#include "runnuble.h"


namespace utility {
namespace threading {
namespace implementation {


CRunnuble::CRunnuble() 
:
    m_is_running(false)
{}


void CRunnuble::initialize() {
    m_is_running = true;
}


void CRunnuble::finalize() {
    m_is_running = false;
}


} // implementation
} // threading
} // utility
