#include "stream_pool.h"

#include "iridium/threading/implementation/thread.h"
#include "iridium/threading/implementation/mutex.h"
#include "iridium/threading/implementation/worker_pool.h"
#include "iridium/threading/synchronized_scope.h"
#include "iridium/io/service.h"
#include "iridium/io/implementation/transmitter.h"
#include "iridium/assert.h"

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
    CSynchronized   (CMutex::create())
{
    threading::IWorkerPool<TransmitterHandler::TSharedPtr>::TWorkerHandlers handlers;

    for (auto i = 0; i < 1; i++)
        handlers.push_back(CStreamHandler::create(*this));

    m_worker_pool = CWorkerPool<TransmitterHandler::TSharedPtr>::create("io_event_handler", handlers);
}


void CStreamPool::CListenerHandler::add(
    IStreamPort::TSharedPtr                 const &stream,
    protocol::IProtocolHandler::TSharedPtr  const &protocol_handler)
{
    LOCK_SCOPE_FAST
    Service::instance().add(stream);
    m_map_stream_protocol[stream->getID()] = protocol_handler;
    LOGT << "fd " << stream->getID();
}


void CStreamPool::CListenerHandler::add(
    IStream::TConstSharedPtr                const &stream,
    protocol::IProtocolHandler::TSharedPtr  const &protocol_handler)
{
    LOCK_SCOPE_FAST
    m_map_stream_protocol[stream->getID()] = protocol_handler;
    LOGT << "fd " << stream->getID();
}


void CStreamPool::CListenerHandler::del(IStream::TSharedPtr const &stream) {
    LOCK_SCOPE_FAST
    m_map_stream_protocol[stream->getID()] = nullptr; // todo: erase
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
        std::list<TransmitterHandler::TSharedPtr> events;

        {
            LOCK_SCOPE_FAST
            for (auto const &event: Service::instance().wait()) {
                assertExists(event->stream, "event stream is null");
                LOGT << "fd " << event->stream->getID();
                auto protocol = m_map_stream_protocol[event->stream->getID()];
                if (!protocol)
                    LOGE << "protocol is NULL, fd " << event->stream->getID();
                events.push_back(
                    TransmitterHandler::create(
                    TransmitterHandler {
                        event,
                        m_map_stream_protocol[event->stream->getID()],
                        CTransmitter::create()
                    }
                ) );
            }
        }

        m_worker_pool->push(events);
    }
}


CStreamPool::CListenerHandler::CStreamHandler::CStreamHandler(CListenerHandler &listener_handler)
:
    m_listener_handler(listener_handler)
{}


void CStreamPool::CListenerHandler::CStreamHandler::initialize() {}


void CStreamPool::CListenerHandler::CStreamHandler::finalize() {}


CStreamPool::CListenerHandler::CStreamHandler::TItems
CStreamPool::CListenerHandler::CStreamHandler::handle(TItems const &handlers_) {
    TItems handlers;

    for (auto const &handler: handlers_) {
        assertExists(handler->protocol_handler, "protocol is null");
        assertExists(handler->transmitter,      "transmitter is null");

        std::set<IStream::TConstSharedPtr> streams{
            handler->transmitter->getReader(),
            handler->transmitter->getWriter()
        };

        if (handler->protocol_handler->redirectStreams(
            handler->transmitter,
            handler->event) &&
            handler->transmitter->transmit(
            handler->event))
        {
            if (handler->transmitter->getReader()->getID() > 0 &&
                streams.count(handler->transmitter->getReader()) == 0)
            {
                m_listener_handler.add(
                    handler->transmitter->getReader(),
                    handler->protocol_handler);
            }

            if (handler->transmitter->getWriter()->getID() > 0 &&
                streams.count(handler->transmitter->getWriter()) == 0)
            {
                m_listener_handler.add(
                    handler->transmitter->getWriter(),
                    handler->protocol_handler);
            }

            handlers.push_back(handler);
        } else {
            // todo: del protocols from map
        }
    }

    return handlers; // -----
}


} // implementation
} // io
} // iridium
