#include "initializer.h"

#include <algorithm>


namespace iridium {
namespace pattern {
namespace implementation {


CInitializer::~CInitializer() {
    if (m_is_initialized)
        CInitializer::finalize();
}


void CInitializer::initialize() {
    if (m_is_initialized)
        return; // ----->
    for (auto const &item: m_items)
        item->initialize();
    m_is_initialized = true;
}


void CInitializer::finalize() {
    if (m_is_initialized)
        for (auto item = m_items.rbegin(); item != m_items.rend(); item++)
            (*item)->finalize();
    m_is_initialized = false;
}


} // implementation
} // pattern
} // iridium
