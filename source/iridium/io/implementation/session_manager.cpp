#include "session_manager.h"

#include "multiplexer.h"

#include "iridium/threading/implementation/thread.h"
#include "iridium/threading/implementation/worker_pool.h"
#include "iridium/items.h"
#include "event.h"


using iridium::threading::Synchronized;
using iridium::threading::implementation::CThread;
using iridium::threading::implementation::CWorkerPool;
using iridium::threading::implementation::CMutex;
using iridium::threading::implementation::CAsyncQueue;
using iridium::createObjects;


namespace iridium {
namespace io {
namespace implementation {


CSessionManager::CSessionManager()
:
    m_protocol_count(0),
    m_event_provider(CMultiplexer::create()),
    m_cache(Cache::create(m_event_provider, m_protocol_count, m_cv)),
    m_worker_pool(CWorkerPool<IEvent::TSharedPtr>::create(
        "stream_worker",
        createObjects<IContextWorker::IHandler, CContextWorkerHandler>(
            std::thread::hardware_concurrency(), m_cache))),
    m_event_provider_thread(
        CThread::create("event_provider",
            CMultiplexerThreadHandler::create(m_worker_pool, m_event_provider))),
    m_event_queue_loop_thread(
        CThread::create("event_queue_loop",
            CEventQueueLoopThreadHandler::create(m_worker_pool)))
{}


void CSessionManager::initialize() {
    m_event_provider->initialize();
    m_event_provider_thread->initialize();
    m_event_queue_loop_thread->initialize();
    m_worker_pool->initialize();
    m_protocol_count = 0;
}


void CSessionManager::finalize() {
//    LOGT << __FUNCTION__;
//    LOGT << 111;
//    m_worker_pool->finalize();
//    LOGT << 222;
//    m_event_provider_thread->finalize();
//    LOGT << 333;
//    m_event_provider->finalize();
//    LOGT << 444;
//    m_event_queue_loop_thread->finalize();
//    LOGT << 555;

//    LOGT << 111;
    m_worker_pool->finalize();
//    LOGT << 222;
    m_event_provider->finalize();
//    LOGT << 333;
    m_event_provider_thread->finalize();
//    LOGT << 444;
    m_event_queue_loop_thread->finalize();
//    LOGT << 555;


    if (m_protocol_count > 0)
        LOGW << "running protocols left: " << static_cast<size_t>(m_protocol_count);
//    LOGT << 666;
}


void CSessionManager::manage(
    IStreamPort::TSharedPtr const &stream,
    IProtocol::TSharedPtr const &protocol)
{
//  if (stream && protocol && stream->getID() > 0) {
//    LOGT << "manage: fd " << stream->getID();
    m_cache->put(stream, protocol);
    m_worker_pool->push(CEvent::create(stream, IEvent::TType::OPEN));
//  } else
//      throw std::runtime_error("session manage error: invalid stream or protocol"); // ----->
    m_protocol_count++;
//    LOGT << "protocol count: " << static_cast<size_t>(m_protocol_count);
}


bool CSessionManager::wait(std::chrono::nanoseconds const &timeout) {
    std::unique_lock<std::mutex> l(m_cv_mutex);

    auto * const protocol_count = &m_protocol_count;

    return m_cv.wait_for(l, timeout,
        [protocol_count] {
//            LOGT << "!!! protocol count: " << static_cast<size_t>(*protocol_count);
            return *protocol_count == 0;
        }
    );
}


CSessionManager::Cache::Cache(
    IMultiplexer::TSharedPtr const    &event_provider,
    std::atomic<size_t>                 &protocol_count,
    std::condition_variable             &cv)
:
    threading::Synchronized (CMutex::create()),
    m_event_provider        (event_provider),
    m_protocol_count        (protocol_count),
    m_cv                    (cv)
{}


void CSessionManager::Cache::put(IStream::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol) {
    LOCK_SCOPE;
    auto context                    = TContext::create();

    context->protocol               = protocol;
    context->events                 = CAsyncQueue<IEvent::TSharedPtr>::create();
    m_map_stream_context[stream]    = context;
    m_map_context_stream[context]   = stream;
//    LOGT << "cache put: fd " << stream->getID();
//    m_event_provider->subscribe(stream);
}


void CSessionManager::Cache::del(TContext::TSharedPtr const &context) {
    LOCK_SCOPE;
    for (auto &i: context->map_id_pipe) {
        auto &pipe = i.second;

        auto reader = std::const_pointer_cast<IStreamReader>(pipe->getReader());
        auto writer = std::const_pointer_cast<IStreamWriter>(pipe->getWriter());

        m_event_provider->unsubscribe(reader);
        m_event_provider->unsubscribe(writer);

        m_map_stream_context.erase(reader);
        m_map_stream_context.erase(writer);

//        reader->finalize();
//        writer->finalize();
    }
    m_map_context_stream.erase(context);

    m_protocol_count--;
//    if (m_protocol_count == 0)
        m_cv.notify_all();

//    LOGT << "!!! protocol count: " << static_cast<size_t>(m_protocol_count);
}


CSessionManager::TContext::TSharedPtr CSessionManager::Cache::get(IEvent::TSharedPtr const &event) {
    LOCK_SCOPE;
    auto i = m_map_stream_context.find(event->getStream());
    if (i == m_map_stream_context.end())
        return nullptr; // ----->
    else {
        auto context = i->second;

        if (event->getType() == IEvent::TType::OPEN) {
            event->getStream()->initialize();
            m_event_provider->subscribe(event->getStream());
        }

        context->events->push(event);
        return context; // ----->
    }
}


CSessionManager::CMultiplexerThreadHandler::CMultiplexerThreadHandler(
    IContextWorker::TSharedPtr const &worker_pool,
    IMultiplexer::TSharedPtr const &event_provider)
:
    m_event_provider(event_provider),
    m_worker_pool   (worker_pool)
{}


void CSessionManager::CMultiplexerThreadHandler::initialize() {}


void CSessionManager::CMultiplexerThreadHandler::finalize() {}


void CSessionManager::CMultiplexerThreadHandler::run(std::atomic<bool> &is_running) {
//    LOGT << "CMultiplexerThreadHandler loop";
    while (is_running)
        m_worker_pool->push(m_event_provider->waitEvents());
}


CSessionManager::CContextWorkerHandler::CContextWorkerHandler(Cache::TSharedPtr const &cache)
:
    m_cache(cache)
{}


CSessionManager::CEventQueueLoopThreadHandler::CEventQueueLoopThreadHandler(IContextWorker::TSharedPtr const &worker_pool)
:
    m_worker_pool(worker_pool)
{}


void CSessionManager::CEventQueueLoopThreadHandler::initialize() {}


void CSessionManager::CEventQueueLoopThreadHandler::finalize() {
//    LOGT << __FUNCTION__;
}


void CSessionManager::CEventQueueLoopThreadHandler::run(std::atomic<bool> &is_running) {
    LOGT << "CEventQueueLoopThreadHandler loop";
    while (is_running)
        m_worker_pool->push(m_worker_pool->pop(is_running));
}


void CSessionManager::CContextWorkerHandler::initialize() {
//    LOGT << "initialize";
}


void CSessionManager::CContextWorkerHandler::finalize() {
//    LOGT << "finalize";
}


CSessionManager::IContextWorker::TOutputItems
CSessionManager::CContextWorkerHandler::handle(
    IContextWorker::TInputItems const &events)
{
    // events to repeat handling
    IContextWorker::TOutputItems result_events;

    for (auto const &event: events) {
//        LOGT << "handle event: " << event->getType();

//        if (event->getType() == IEvent::TType::OPEN)
//            event->getStream()->initialize();

//        auto id = event->getStream()->getID();
//        LOGT << "handle: fd " << id << " " << event->getType();

        if (auto context = m_cache->get(event)) {
            bool is_delete_context = false;
            try {
                auto context_events = context->events->pop(false);
                for (auto &context_event: context_events) {
                    if (!checkOneOf(context_event->getType(), IEvent::TType::READ, IEvent::TType::WRITE) &&
                        !context->protocol->control(context_event, context->map_id_pipe))
                    {
                        is_delete_context = true;
                        break; // --->
                    }

                    for (auto const &i: context->map_id_pipe) {
                        auto &pipe = i.second;
//                        context_event->setType(IEvent::TType::READ);

                        threading::sleep(500);
                        bool result = pipe->transmit(context_event);
                        LOGT << "TRANSMIT 1 result '" << static_cast<bool>(result) << "'";

                        if (result && checkOneOf(context_event->getType(), IEvent::TType::READ, IEvent::TType::WRITE)) {
                            // repeat event
//                            LOGT << "handle: fd " << event->getStream()->getID() << " " << event->getType() << " repeat";
                            result_events.push_back(event);
                        }

                        if (!result) {
                            result = pipe->transmit(context_event);
                            LOGT << "TRANSMIT 2 result '" << static_cast<bool>(result) << "'";
                        }

//                        if (context_event->getType() == IEvent::TType::OPEN)
//                            context_event->setType(IEvent::TType::READ);
//                        if (!result) {
////                            LOGT << "delete context due transmitting error";
//                            is_delete_context = true;
//                        }
                    }

                    if (!context->protocol->control(context_event, context->map_id_pipe) || is_delete_context) {
//                        LOGT << "delete context due protocol";
                        is_delete_context = true;
                        context_event->setType(IEvent::TType::CLOSE);
                        context->protocol->control(context_event, context->map_id_pipe);
                        break; // --->
                    }
                }
            } catch (std::exception const &e) {
                LOGE << e.what();
                is_delete_context = true;
            }

            if (is_delete_context) {
//                LOGT << "delete context, buffer:\n";
                m_cache->del(context);
            }
//            threading::sleep(100);
        }
    }
//    threading::sleep(100);
    return result_events; // ----->
}


} // implementation
} // io
} // iridium
