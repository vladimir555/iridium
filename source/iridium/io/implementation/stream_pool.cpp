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
using iridium::threading::Synchronized;


#include "iridium/logging/logger.h"
namespace iridium {
namespace io {
namespace implementation {


CStreamPool::CStreamPool()
:
    Synchronized        (CMutex::create()),
    m_is_initialized    (false),
    m_listener          (CListener::create())
{}


void CStreamPool::add(protocol::ISession::TSharedPtr const &session) {
    if (!m_is_initialized)
        return;
}


void CStreamPool::del(protocol::ISession::TSharedPtr const &/*session*/) {
    if (!m_is_initialized)
        return;
}


void CStreamPool::initialize() {
    LOGT;
    m_listener->initialize();

    m_context_handler_pool = CWorkerPool<TContext::TSharedPtr>::create<CContextHandler>("event_handler", 4, shared_from_this());
    m_context_handler_pool->initialize();

    m_listener_thread = CThread::create("listener_handler", CListenerHandler::create(shared_from_this()));
    m_listener_thread->initialize();

    m_is_initialized = true;
}


void CStreamPool::finalize() {
    m_listener_thread->finalize();
    m_context_handler_pool->finalize();
    m_listener->finalize();
}


CStreamPool::TContext::TSharedPtr CStreamPool::getContext(const ISession::TSharedPtr &session_, const Event::TSharedPtr &event) {
    LOCK_SCOPE;
    auto session = session_;
    if (!session && event && event->stream)
        session = m_map_stream_session[event->stream];

    auto i =  m_map_session_events.find(session);
    if ( i == m_map_session_events.end() || i->second->empty()) {
        TContext context{session, CAsyncQueue<Event::TSharedPtr>::create()};
        m_map_session_events[context.m_session] = context.m_events;

        if (event)
            context.m_events->push(event);
        else {
//            auto stream = session->getStream(m_listener);
//            context.m_events->push(Event::create(Event::TType::OPEN, stream, m_listener));
//            m_map_stream_session[stream] = session;
        }

        return TContext::create(context); // ---->
    } else {
        i->second->push(event);

        return {}; // ----->
    }
    return {}; // ----->
}


CStreamPool::CContextHandler::CContextHandler(CStreamPool::TWeakPtr const &stream_pool)
:
    m_stream_pool(stream_pool)
{}


void CStreamPool::CContextHandler::initialize() {}


void CStreamPool::CContextHandler::finalize() {}


void CStreamPool::CContextHandler::handle(TItems const &contexts) {
    auto stream_pool = m_stream_pool.lock();
    for (auto const &context: contexts) {
        for (;;) {
            auto events = context->m_events->pop(false);
            if (events.empty()) {
                break;
            } else {
                for (auto const &event: events) {
                    LOGT << "event " << event->type << " fd " << event->stream->getID();
                    if (auto pipe = context->m_session->getPipe(event)) {
                        LOGT << "event " << event->type << " fd " << event->stream->getID() << " transmit";
                        // EOF -> redirect streams
                        if (!pipe->transmit(event)) {
                            LOGT << "event " << event->type << " fd " << event->stream->getID() << " transmit EOF";
                            event->stream   = std::const_pointer_cast<IStream>(IStream::TConstSharedPtr(pipe->getReader()));
                            event->type     = IListener::Event::TType::EOF_;
                            context->m_session->getPipe(event);
                        }
                    } else {
                        LOGT << "event " << event->type << " fd " << event->stream->getID() << " del session";
                    }
                    LOGT << "event " << event->type << " fd " << event->stream->getID() << " done";
                }
            }
        };
    }
}


CStreamPool::CListenerHandler::CListenerHandler(CStreamPool::TWeakPtr const &stream_pool)
:
    m_stream_pool(stream_pool)
{}


void CStreamPool::CListenerHandler::initialize() {
}


void CStreamPool::CListenerHandler::finalize() {
}


void CStreamPool::CListenerHandler::run(std::atomic<bool> &is_running) {
    while (is_running) {
        auto stream_pool = m_stream_pool.lock();
        for (auto const &event: stream_pool->m_listener->wait()) {
            auto context = stream_pool->getContext(nullptr, event);
            if (context)
                stream_pool->m_context_handler_pool->push(context);
        }
    }
}


} // implementation
} // io
} // iridium
