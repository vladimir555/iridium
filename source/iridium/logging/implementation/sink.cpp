#include "sink.h"



#include "iridium/threading/implementation/worker.h"


using iridium::threading::IWorkerHandler;
using iridium::threading::implementation::CWorker;


namespace iridium {
namespace logging {
namespace implementation {


CSink::CSink(TEvent::TLevel const &level, IWorkerHandler<TEvent>::TSharedPtr const &worker_handler)
:
    m_level(level),
    m_worker(CWorker<TEvent>::create("sink", worker_handler))
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
} // iridium
