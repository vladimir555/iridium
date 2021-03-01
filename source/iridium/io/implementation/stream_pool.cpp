#include "stream_pool.h"

#include "iridium/threading/implementation/thread.h"
#include "iridium/threading/implementation/mutex.h"
#include "iridium/threading/implementation/worker_pool.h"
#include "iridium/threading/implementation/async_queue.h"
#include "iridium/threading/synchronized_scope.h"
#include "iridium/io/implementation/listener.h"
#include "iridium/io/implementation/pipe.h"

#include "iridium/assert.h"
#include "iridium/items.h"

#include <set>


using iridium::threading::implementation::CThread;
using iridium::threading::implementation::CMutex;
using iridium::threading::implementation::CWorkerPool;
using iridium::threading::implementation::CAsyncQueue;
//using iridium::io::net::


#include "iridium/logging/logger.h"
namespace iridium {
namespace io {
namespace implementation {


CStreamPool::CStreamPool() {
    auto listener_handler   = CListenerHandler::create(CListener::create());
    m_listener_handler      = listener_handler;
    m_listener_thread       = CThread::create(listener_handler, "io_event_listener");
}


void CStreamPool::add(protocol::ISession::TSharedPtr const &session) {
    m_listener_handler->add(session);
}


void CStreamPool::del(protocol::ISession::TSharedPtr const &session) {
    m_listener_handler->del(session);
}


void CStreamPool::initialize() {
    m_listener_thread->initialize();
}


void CStreamPool::finalize() {
    m_listener_thread->finalize();
}


CStreamPool::CListenerHandler::CListenerHandler(IListener::TSharedPtr const &listener)
:
    Synchronized (CMutex::create()),
    m_listener   (listener),
    m_sessions   (Sessions::create()),
    m_worker_pool(CWorkerPool<Event::TSharedPtr>::create("io_event_handler",
        createObjects<TWorkerPool::TWorkerHandler, CEventHandler>(1, m_sessions)))
{}


void CStreamPool::CListenerHandler::add(protocol::ISession::TSharedPtr const &session) {
    LOCK_SCOPE
    auto event = Event::create(Event{ {Event::TType::OPEN, nullptr, m_listener}, session} );
    m_worker_pool->push(event);
}


void CStreamPool::CListenerHandler::del(protocol::ISession::TSharedPtr const &session) {
    LOCK_SCOPE
    LOGT;
//    m_sessions->m_streams_to_delete->push(stream);
}


void CStreamPool::CListenerHandler::initialize() {
    LOCK_SCOPE
    m_listener->initialize();
//    m_sessions = Sessions::create();
    m_worker_pool->initialize();
}


void CStreamPool::CListenerHandler::finalize() {
    LOCK_SCOPE
    m_sessions.reset();
    m_worker_pool->finalize();
    m_listener->finalize();
}


void CStreamPool::CListenerHandler::run(std::atomic<bool> &is_running) {
    while (is_running) {
        for (auto const &event_: m_listener->wait()) {
            auto event = Event::create(Event
                { {event_->type, event_->stream, event_->listener}, m_sessions->m_map_stream_session.get(event_->stream) });
            m_worker_pool->push(event);
        }
    }
}


CStreamPool::CListenerHandler::Sessions::Sessions()
//:
//    m_streams_to_delete(CAsyncQueue<IStream::TSharedPtr>::create())
{}


CStreamPool::CListenerHandler::CEventHandler::CEventHandler(Sessions::TSharedPtr const &sessions)
:
    m_sessions(sessions)
{}


void CStreamPool::CListenerHandler::CEventHandler::initialize() {
    LOGT;
}


void CStreamPool::CListenerHandler::CEventHandler::finalize() {
    LOGT;
}


CStreamPool::CListenerHandler::CEventHandler::TItems
CStreamPool::CListenerHandler::CEventHandler::handle(TItems const &events_) {
    TItems events;

    for (auto const &event: events_) {
        if (auto session = event->session) {
            if(!event->stream) {
                event->stream = session->getStream(event->listener);
                m_sessions->m_map_stream_session.set(event->stream, session);
            }

            LOGT << "event fd " << event->stream->getID() << " " << event->type;

            if (auto pipe = session->getPipe(event)) {
                // EOF -> redirect streams
                if (!pipe->transmit(event)) {
                    event->stream   = std::const_pointer_cast<IStream>(IStream::TConstSharedPtr(pipe->getReader()));
                    event->type     = IListener::Event::TType::EOF_;
                    session->getPipe(event);
                }

                if (pipe && pipe->getReader() && pipe->getReader()->getID() > 0)
                    m_sessions->m_map_stream_session.set(pipe->getReader(), session);
                if (pipe && pipe->getWriter() && pipe->getWriter()->getID() > 0)
                    m_sessions->m_map_stream_session.set(pipe->getWriter(), session);
            } else {
                LOGT << "del session fd " << event->stream->getID();
            }
        }
    }

    return events; // ----->
}


} // implementation
} // io
} // iridium
