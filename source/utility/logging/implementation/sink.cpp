#include "sink.h"


using utility::threading::IWorker;


namespace utility {
namespace logging {
namespace implementation {


CSink::CSink(TEvent::TLevel const &level, IWorker<TEvent>::TSharedPtr worker)
:
    m_level (level),
    m_worker(worker)
{}


void CSink::initialize() {
    m_worker->initialize();
}


void CSink::finalize() {
    m_worker->finalize();
}


void CSink::log(TEvent const &e) {
    if (e.level >= m_level)
        m_worker->push(e);
}


} // implementation
} // logger
} // utility
