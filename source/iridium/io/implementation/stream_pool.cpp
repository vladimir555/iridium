#include "stream_pool.h"

#include "iridium/threading/implementation/thread.h"
#include "iridium/threading/implementation/mutex.h"
#include "iridium/threading/implementation/worker_pool.h"
#include "iridium/threading/synchronized_scope.h"
#include "iridium/io/service.h"
#include "iridium/io/implementation/transmitter.h"
#include "iridium/assert.h"
#include "iridium/items.h"

#include <set>


using iridium::threading::implementation::CThread;
using iridium::threading::implementation::CMutex;
using iridium::threading::implementation::CWorkerPool;


#include "iridium/logging/logger.h"
namespace iridium {
namespace io {
namespace implementation {


CStreamPool::CStreamPool() {
    auto runnable   = CListenerHandler::create();
    m_runnable      = runnable;
    m_thread        = CThread::create(runnable, "io_event_listener");
}


void CStreamPool::add(
    IStreamPort::TSharedPtr                 const &stream,
    protocol::IProtocolHandler::TSharedPtr  const &protocol_handler)
{
    m_runnable->add(stream, protocol_handler);
}


void CStreamPool::del(IStream::TSharedPtr const &stream) {
    m_runnable->del(stream);
}


void CStreamPool::initialize() {
    m_thread->initialize();
}


void CStreamPool::finalize() {
    m_thread->finalize();
}


CStreamPool::CListenerHandler::CListenerHandler()
:
    Synchronized(CMutex::create())
{
    threading::IWorkerPool<Event::TSharedPtr>::TWorkerHandlers handlers;

    for (auto i = 0; i < 1; i++)
        handlers.push_back(CStreamHandler::create(m_transmitters));

    m_worker_pool = CWorkerPool<Event::TSharedPtr>::create("io_event_handler", handlers);
}


void CStreamPool::CListenerHandler::add(
    IStreamPort::TSharedPtr                 const &stream,
    protocol::IProtocolHandler::TSharedPtr  const &protocol_handler)
{
    LOCK_SCOPE_FAST
    try {
        auto transmitter = CTransmitterHandler::create(protocol_handler, m_transmitters);

        transmitter->transmit(Event::create(Event::TType::OPEN, stream));
        m_transmitters.set(stream, transmitter);

        LOGT << "fd " << stream->getID();
    } catch (std::exception const &e) {
        LOGE << e.what();
    }
}


void CStreamPool::CListenerHandler::del(IStream::TSharedPtr const &stream) {
    LOCK_SCOPE_FAST
    m_transmitters.del(stream);
    LOGT << "fd " << stream->getID();
}


void CStreamPool::CListenerHandler::initialize() {
    LOCK_SCOPE_FAST
    m_worker_pool->initialize();
}


void CStreamPool::CListenerHandler::finalize() {
    LOCK_SCOPE_FAST
    m_worker_pool->finalize();
}


void CStreamPool::CListenerHandler::run(std::atomic<bool> &is_running) {
    while (is_running) {
        m_worker_pool->push(Service::instance().wait());
    }
}


CStreamPool::CListenerHandler::CTransmitterHandler::CTransmitterHandler(
    protocol::IProtocolHandler::TSharedPtr const &protocol_handler,
    TTransmitters &transmitters)
:
    threading::Synchronized(threading::implementation::CMutex::create()),
    m_transmitters      (transmitters),
    m_protocol_handler  (protocol_handler)
{}


CStreamPool::CListenerHandler::CTransmitterHandler::~CTransmitterHandler() {
    // todo: check null
    // todo: m_transmitters.del
}


void CStreamPool::CListenerHandler::CTransmitterHandler::set(
    const IStreamReader::TSharedPtr &reader,
    const IStreamWriter::TSharedPtr &writer)
{
    if (reader)
        m_transmitters.set(reader, shared_from_this());
    if (writer)
        m_transmitters.set(writer, shared_from_this());
    CTransmitter::set(reader, writer);
}


bool CStreamPool::CListenerHandler::CTransmitterHandler::transmit(const Event::TSharedPtr &event) {
    LOCK_SCOPE;
    if (!checkOneOf(event->type, Event::TType::ERROR, Event::TType::CLOSE)  &&
        m_protocol_handler->redirectStreams(shared_from_this(), event)      &&
        CTransmitter::transmit(event)
    ) {
        threading::sleep(1000);
        return true; // ----->
    } else {
        // todo: m_transmitters.del
        return false;
    }
}


CStreamPool::CListenerHandler::CStreamHandler::CStreamHandler(TTransmitters &transmitters)
:
    m_transmitters(transmitters)
{}


void CStreamPool::CListenerHandler::CStreamHandler::initialize() {}


void CStreamPool::CListenerHandler::CStreamHandler::finalize() {}


CStreamPool::CListenerHandler::CStreamHandler::TItems
CStreamPool::CListenerHandler::CStreamHandler::handle(TItems const &events_) {
    TItems events;

    for (auto const &event: events_) {
        try {
            if (m_transmitters.get(event->stream)->transmit(event))
                events.push_back(event);
        } catch (std::exception const &e) {
            LOGE << e.what();
            m_transmitters.del(event->stream);
        }
    }

    return events; // -----
}


} // implementation
} // io
} // iridium
