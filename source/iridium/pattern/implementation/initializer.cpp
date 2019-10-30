/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "initializer.h"

#include <algorithm>


namespace iridium {
namespace pattern {
namespace implementation {


//CInitializer::CInitializer(std::list<IInitializable::TSharedPtr> const &items)
//:
//    m_is_initialized    (false),
//    m_items             (items)
//{}


CInitializer::~CInitializer() {
    finalize();
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
