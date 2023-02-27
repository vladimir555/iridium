#include "session_manager.h"

#include "multiplexer.h"

#include "iridium/threading/implementation/thread.h"
#include "iridium/threading/implementation/worker_pool.h"
#include "iridium/threading/implementation/recursive_mutex.h"
#include "iridium/io/implementation/pipe.h"
#include "iridium/items.h"
#include "event.h"


using iridium::threading::Synchronized;
using iridium::threading::implementation::CThread;
using iridium::threading::implementation::CWorkerPool;
using iridium::threading::implementation::CMutex;
using iridium::threading::implementation::CRecursiveMutex;
using iridium::threading::implementation::CAsyncQueue;
using iridium::createObjects;


namespace iridium {
namespace io {
namespace implementation {


std::chrono::seconds const DEFAULT_TIMEOUT(10);


//CSessionManager::ContextManager::~ContextManager() {
//}


CSessionManager::ContextManager::Context::Context(
    IMultiplexer::TSharedPtr    const &multiplexer,
    ContextManager *    const context_manager,
    IProtocol::TSharedPtr       const &protocol)
:
    m_multiplexer       (multiplexer),
//    m_context_manager   (context_manager),
    m_protocol          (protocol),
    m_events            (CAsyncQueue<IEvent::TSharedPtr>::create())
{}


CSessionManager::ContextManager::Context::~Context() {
    for (auto const &name_pipe: m_map_name_pipe) {
        if (name_pipe.second->getReader())
            m_multiplexer->unsubscribe(name_pipe.second->getReader());
        if (name_pipe.second->getWriter())
            m_multiplexer->unsubscribe(name_pipe.second->getWriter());
    }
}


IPipe::TSharedPtr CSessionManager::ContextManager::Context::createPipe(std::string const &name) {
    removePipe(name);

    auto pipe = CPipe::create();

    m_map_name_pipe[name] = pipe;
    return pipe;
}


void CSessionManager::ContextManager::Context::removePipe(std::string const &name) {
    if (auto pipe = findPipe(name)) {
        if (pipe->getReader()) {
            m_multiplexer->unsubscribe(pipe->getReader());            
        }
        if (pipe->getWriter()) {
            m_multiplexer->unsubscribe(pipe->getWriter());
        }
    }
}


void CSessionManager::ContextManager::Context::updateStream(
    std::string         const &name,
    IStream::TSharedPtr const &stream,
    IEvent::TType       const &type)
{
    if (auto pipe = findPipe(name)) {
        IStream::TConstSharedPtr prev_stream;

        if (type == IEvent::TType::READ)
            prev_stream = pipe->getReader();
        else
        if (type == IEvent::TType::WRITE)
            prev_stream = pipe->getWriter();
        else
            throw std::runtime_error(
                "context update error: pipe '" + name +
                "', stream type '" + convert<string>(type) + "' is wrong"); // ----->

        if (prev_stream != stream) {
            if (stream) {
                auto stream_pipe  = m_map_stream_pipe.find(stream);
                if  (stream_pipe == m_map_stream_pipe.end()) {
                    stream->initialize();
                    m_multiplexer->subscribe(stream);
                    m_map_stream_pipe[stream] = pipe;
                }
            }

            if (type == IEvent::TType::READ)
                pipe->set(
                    std::dynamic_pointer_cast<IStreamReader>(stream),
                    std::const_pointer_cast<IStreamWriter>(pipe->getWriter()));
            if (type == IEvent::TType::WRITE)
                pipe->set(
                    std::const_pointer_cast<IStreamReader>(pipe->getReader()),
                    std::dynamic_pointer_cast<IStreamWriter>(stream));

            if (stream->getID() > 0) {
//                LOGT << "pipe event: " << stream->getID() << " " << type;
                m_events->push(CEvent::create(stream, type));
            }
        }
    } else
        throw std::runtime_error("context update error: pipe '" + name + "' not found"); // ----->
}


void CSessionManager::ContextManager::Context::updateReader(
    std::string const &name, IStreamReader::TSharedPtr const &reader)
{
    updateStream(name, reader, IEvent::TType::READ);
}


void CSessionManager::ContextManager::Context::updateWriter(
    std::string const &name, IStreamWriter::TSharedPtr const &writer)
{
    updateStream(name, writer, IEvent::TType::WRITE);
}


CSessionManager::ContextManager::ContextManager(IMultiplexer::TSharedPtr const &multiplexer)
:
    m_multiplexer(multiplexer)
{}


CSessionManager::ContextManager::~ContextManager() {
}


IPipe::TSharedPtr CSessionManager::ContextManager::Context::findPipe(std::string const &name) const {
    auto i = m_map_name_pipe.find(name);
    if (i == m_map_name_pipe.end())
        return nullptr; // ----->
    else
        return i->second; // ----->
}


IPipe::TSharedPtr CSessionManager::ContextManager::Context::findPipe(IStream::TSharedPtr const &stream) const {
    auto stream_pipe  = m_map_stream_pipe.find(stream);
    if  (stream_pipe == m_map_stream_pipe.end())
        return nullptr;
    else
        return stream_pipe->second;
}


void CSessionManager::ContextManager::Context::pushEvent(IEvent::TSharedPtr const &event) {
    m_events->push(event);
}


std::list<IEvent::TSharedPtr> CSessionManager::ContextManager::Context::popEvents() {
    auto events = m_events->pop(false); // ----->
    return events;
}


IProtocol::TSharedPtr CSessionManager::ContextManager::Context::getProtocol() const {
    return m_protocol; // ----->
}


CSessionManager::CSessionManager()
:
    m_multiplexer(
        CMultiplexer::create()),
    m_context_manager(
        ContextManager::create(m_multiplexer)),
    m_context_worker(
        CWorkerPool<IEvent::TSharedPtr>::create(
            "stream_worker",
            createObjects<IContextWorker::IHandler, CContextWorkerHandler>(
                std::thread::hardware_concurrency(), m_context_manager.get()))),
    m_multiplexer_thread(
        CThread::create("event_multiplexer",
            CMultiplexerThreadHandler::create(m_context_worker, m_multiplexer, m_context_manager.get()))),
    m_event_repeater_thread(
        CThread::create("event_repeater",
            CEventRepeaterHandler::create(m_context_worker)))
{}


void CSessionManager::initialize() {
    m_multiplexer->initialize();
    m_multiplexer_thread->initialize();
    m_event_repeater_thread->initialize();
    m_context_worker->initialize();
//    m_session_count = 0;
}


void CSessionManager::finalize() {
    m_context_worker->finalize();
    m_multiplexer_thread->finalize();
    m_event_repeater_thread->finalize();
    m_multiplexer->finalize();

//    if (m_session_count > 0)
//        LOGW << "running session count left: " << static_cast<size_t>(m_session_count);
}


void CSessionManager::manage(IStreamPort::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol) {
    if (stream && protocol) {
//    LOGT << "manage: fd " << stream->getID();
        m_context_manager->updateContext(
            ContextManager::Context::create(m_multiplexer, m_context_manager.get(), protocol), stream);
        m_context_worker->push(CEvent::create(stream, IEvent::TType::OPEN));
    } else
        throw std::runtime_error("session manage error: null stream or protocol"); // ----->
//    m_session_count++;
//    LOGT << "protocol count: " << static_cast<size_t>(m_protocol_count);
}


CSessionManager::ContextManager::Context::TSharedPtr CSessionManager::ContextManager::acquireContext(
    IEvent::TSharedPtr const &event)
{
    LOCK_SCOPE;
    m_map_stream_timestamp[event->getStream()] = std::chrono::system_clock::now();
    auto stream_context  = m_map_stream_context.find(event->getStream());
    if  (stream_context != m_map_stream_context.end()) {
        auto context = stream_context->second;
        context->pushEvent(event);
        auto i  = m_acquired_contexts.find(context);
        if  (i == m_acquired_contexts.end()) {
            m_acquired_contexts.insert(context);
            return context; // ----->
        }
    }

    return nullptr;
}


void CSessionManager::ContextManager::releaseContext(Context::TSharedPtr const &context) {
    LOCK_SCOPE;
    m_acquired_contexts.erase(context);
}


void CSessionManager::ContextManager::updateContext(Context::TSharedPtr const &context, IStream::TSharedPtr const &stream) {
    LOCK_SCOPE;
    m_map_stream_context[stream] = context;
    m_map_context_streams[context].insert(stream);
    m_map_stream_timestamp[stream] = std::chrono::system_clock::now();
}


void CSessionManager::ContextManager::removeContext(Context::TSharedPtr const &context) {
    LOCK_SCOPE;
    for (auto const &stream: m_map_context_streams.at(context)) {
        m_map_stream_context.erase(stream);
        m_map_stream_timestamp.erase(stream);
    }
//    auto result =
    m_map_context_streams.erase(context);
//    if (result > 0) {
//        m_protocol_count--;
//        m_cv.notify_one();
//    }
}


std::list<IStream::TConstSharedPtr> CSessionManager::ContextManager::getOutdatedStreams() {
    auto timestamp = std::chrono::system_clock::now() - DEFAULT_TIMEOUT;

    LOCK_SCOPE;

    std::list<IStream::TConstSharedPtr> result;
    for (auto const &stream_timestamp: m_map_stream_timestamp)
        if (stream_timestamp.second < timestamp)
            result.push_back(stream_timestamp.first);

//    string s;
//    for (auto const &i: result)
//        s += convert<string>(i->getID()) + " ";
//    LOGT << "outdated streams: " << s;

    return result; // ----->
}


void CSessionManager::ContextManager::updateStreamTimestamp(IStream::TConstSharedPtr const &stream) {
    auto timestamp = std::chrono::system_clock::now() - DEFAULT_TIMEOUT;

    LOCK_SCOPE;

    m_map_stream_timestamp[stream] = timestamp;
}


CSessionManager::CMultiplexerThreadHandler::CMultiplexerThreadHandler(
    IContextWorker::TSharedPtr  const &context_worker,
    IMultiplexer::TSharedPtr    const &multiplexer,
    ContextManager * const context_manager
//    ContextManager::TWeakPtr   const &context_manager
)
:
    m_multiplexer       (multiplexer),
    m_context_worker    (context_worker),
    m_context_manager   (context_manager)
{}


void CSessionManager::CMultiplexerThreadHandler::initialize() {}


void CSessionManager::CMultiplexerThreadHandler::finalize() {}


void CSessionManager::CMultiplexerThreadHandler::run(std::atomic<bool> &is_running) {
    while (is_running) {
        m_context_worker->push(m_multiplexer->waitEvents());
        for (auto const &stream: m_context_manager->getOutdatedStreams())
            m_context_worker->push(CEvent::create(std::const_pointer_cast<IStream>(stream), IEvent::TType::TIMEOUT));
    }
}


CSessionManager::CContextWorkerHandler::CContextWorkerHandler(ContextManager * const context_manager)
:
    m_context_manager(context_manager)
{}


CSessionManager::CEventRepeaterHandler::CEventRepeaterHandler(IContextWorker::TSharedPtr const &context_worker)
:
    m_context_worker(context_worker)
{}


void CSessionManager::CEventRepeaterHandler::initialize() {}


void CSessionManager::CEventRepeaterHandler::finalize() {}


void CSessionManager::CEventRepeaterHandler::run(std::atomic<bool> &is_running) {
    while (is_running) {
        auto events = m_context_worker->pop();
//        for (auto const &event: events)
//            LOGT << "loop: " << event->getStream()->getID() << " " << event->getType();
        m_context_worker->push(events);
    }
}


void CSessionManager::CContextWorkerHandler::initialize() {}


void CSessionManager::CContextWorkerHandler::finalize() {}


CSessionManager::IContextWorker::TOutputItems
CSessionManager::CContextWorkerHandler::handle(
    IContextWorker::TInputItems const &events)
{
    // events to repeat handling
    IContextWorker::TOutputItems events_to_repeat;

    for (auto const &event: events) {
//        LOGT << "event: " << event->getType();
        if (auto context = m_context_manager->acquireContext(event)) {
            bool is_valid_context = false;
            try {
                auto context_events = context->popEvents();

                // remove duplicates
                {
                    auto i = std::unique(context_events.begin(), context_events.end(),
                        [] (auto const &l, auto const &r) {
                            return
                                l && r &&
                                l->getStream()->getID() == r->getStream()->getID() &&
                                l->getType() == r->getType();
                        });
                    context_events.erase(i, context_events.end());
                }

                for (auto const &context_event: context_events) {
//                    is_valid_context = context->getProtocol()->control(context_event, context);

                    is_valid_context = context->getProtocol()->control(context_event, context);
//                    LOGT << "is_valid_context = " << is_valid_context;
                    if (context_event->getType() == IEvent::TType::OPEN) {
//                        is_valid_context = context->getProtocol()->control(context_event, context);
                        continue;
                    }

//                    LOGT << "event: " << context_event->getStream()->getID() << " " << context_event->getType();

                    auto pipe = context->findPipe(context_event->getStream());
                    if  (pipe) {
                        bool is_transmitted = pipe->transmit(context_event);
                        m_context_manager->updateStreamTimestamp(context_event->getStream());
//                        LOGT << "tramsmit: " << is_transmitted;
//                        result = false;
                        if (is_transmitted &&
                            checkOneOf(
                                event->getType(),
                                IEvent::TType::READ,
                                IEvent::TType::WRITE,
                                IEvent::TType::CLOSE) &&
                            event->getStream()->getID() > 0)
                        {
//                            LOGT << "repeat: " << context_event->getStream()->getID() << " " << context_event->getType();
                            context->pushEvent(context_event);
                        }
                    } else {
                        // todo: throw
                        LOGE << "pipe not found: fd " << context_event->getStream()->getID();
                    }
                    is_valid_context = context->getProtocol()->control(context_event, context);
//                    LOGT << "is_valid_context = " << is_valid_context;
                }
            } catch (std::exception const &e) {
                LOGE << e.what();
                is_valid_context = false;
            }

            // todo: check timeout for not valid but transmitting context
            if (is_valid_context) {
                events_to_repeat.splice(events_to_repeat.end(), context->popEvents());
            } else {
                m_context_manager->removeContext(context);
            }
//            threading::sleep(100);
            m_context_manager->releaseContext(context);
        }
    }
//    threading::sleep(100);
//    for (auto const &event: events_to_repeat)
//        LOGT << "repeat: " << event->getStream()->getID() << " " << event->getType();
    return events_to_repeat; // ----->
}


} // implementation
} // io
} // iridium
