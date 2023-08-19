#include "sink.h"

#include "iridium/threading/implementation/worker.h"


using iridium::threading::IWorker;
using iridium::threading::implementation::CWorkerPusher;


namespace iridium {
namespace logging {
namespace implementation {


CSinkAsync::CSinkAsync(ISink::TSharedPtr const &sink)
:
    m_worker(CWorkerPusher<TEvent::TConstSharedPtr>::create("sink_async", CWorkerHandler::create(sink)))
{}


void CSinkAsync::initialize() {
    m_worker->initialize();
}


void CSinkAsync::finalize() {
    m_worker->finalize();
}


void CSinkAsync::log(TEvent::TConstSharedPtr const &e) {
    m_worker->push(e);
}


void CSinkAsync::flush() {}


CSinkAsync::CWorkerHandler::CWorkerHandler(ISink::TSharedPtr const &sink)
:
    m_sink(sink)
{}


void CSinkAsync::CWorkerHandler::initialize() {
    m_sink->initialize();
}


void CSinkAsync::CWorkerHandler::finalize() {
    m_sink->finalize();
}


void CSinkAsync::CWorkerHandler::handle(TInputItems const &logger_events) {
    for (auto const &e: logger_events)
        m_sink->log(e);
    m_sink->flush();
}


} // implementation
} // logger
} // iridium
