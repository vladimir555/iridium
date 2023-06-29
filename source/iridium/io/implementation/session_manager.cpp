#include "session_manager.h"

#include "multiplexer.h"

#include "iridium/logging/logger.h"
#include "iridium/threading/implementation/thread.h"
#include "iridium/threading/implementation/worker_pool.h"
#include "iridium/io/implementation/context_manager.h"
#include "iridium/items.h"
#include "event.h"


using iridium::threading::Synchronized;
using iridium::threading::implementation::CThread;
using iridium::threading::implementation::CWorkerPool;
using iridium::threading::implementation::CAsyncQueue;
using iridium::createObjects;


namespace iridium {
namespace io {
namespace implementation {


CSessionManager::CSessionManager()
:
    m_multiplexer(
        CMultiplexer::create()),
    m_context_manager(
        CContextManager::create()),
    m_context_worker(
        CWorkerPool<Event::TSharedPtr>::create(
            "context",
            createObjects<IContextWorker::IHandler, CContextWorkerHandler>(
                std::thread::hardware_concurrency(), m_context_manager, m_multiplexer))),
    m_multiplexer_thread(
        CThread::create(
            "multiplexer",
            CMultiplexerThreadHandler::create(m_context_worker, m_context_manager, m_multiplexer))),
    m_event_repeater_thread(
        CThread::create(
            "repeater",
            CEventRepeaterHandler::create(m_context_worker)))
{}


void CSessionManager::initialize() {
    m_multiplexer->initialize();
    m_multiplexer_thread->initialize();
    m_event_repeater_thread->initialize();
    m_context_worker->initialize();
}


void CSessionManager::finalize() {
//    LOGT << "CSessionManager::finalize ...";
    m_context_worker->finalize();
    m_event_repeater_thread->finalize();
    m_multiplexer->finalize();
    m_multiplexer_thread->finalize();
//    LOGT << "CSessionManager::finalize OK";
}


void CSessionManager::manage(IStreamPort::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol) {
    if (stream && protocol) {
        m_context_manager->createContext(stream, protocol);
        m_context_worker->push(Event::create(stream, Event::TOperation::OPEN, Event::TStatus::BEGIN));
    } else
        throw std::runtime_error("session manage error: null stream or protocol"); // ----->
}


CSessionManager::CMultiplexerThreadHandler::CMultiplexerThreadHandler(
    IContextWorker::TSharedPtr  const &context_worker,
    IContextManager::TSharedPtr const &context_manager,
    IMultiplexer::TSharedPtr    const &multiplexer)
:
    m_context_worker    (context_worker),
    m_context_manager   (context_manager),
    m_multiplexer       (multiplexer)
{}


void CSessionManager::CMultiplexerThreadHandler::initialize() {}


void CSessionManager::CMultiplexerThreadHandler::finalize() {}


void CSessionManager::CMultiplexerThreadHandler::run(std::atomic<bool> &is_running) {
    while (is_running) {
        LOGT << "multiplexer wait ...";
        auto events = m_multiplexer->waitEvents();
        LOGT << "multiplexer wait OK, events: " << events.size();
//        m_context_worker->push(m_multiplexer->waitEvents());
        for (auto const &event: events)
            LOGT << "multiplexer event: " << event->operation << " " << event->status << " " << event->stream->getID();
        m_context_worker->push(events);
        m_context_worker->push(m_context_manager->checkOutdatedStreams());
    }
}


CSessionManager::CEventRepeaterHandler::CEventRepeaterHandler(IContextWorker::TSharedPtr const &context_worker)
:
    m_context_worker(context_worker)
{}


void CSessionManager::CEventRepeaterHandler::initialize() {}


void CSessionManager::CEventRepeaterHandler::finalize() {}


void CSessionManager::CEventRepeaterHandler::run(std::atomic<bool> &is_running) {
    while (is_running) {
        auto events = m_context_worker->pop();
        m_context_worker->push(events);
    }
}


CSessionManager::CContextWorkerHandler::CContextWorkerHandler(
    IContextManager::TSharedPtr const &context_manager,
    IMultiplexer::TSharedPtr    const &multiplexer)
:
    m_context_manager   (context_manager),
    m_multiplexer       (multiplexer)
{}


void CSessionManager::CContextWorkerHandler::initialize() {}


void CSessionManager::CContextWorkerHandler::finalize() {}


CSessionManager::IContextWorker::TOutputItems
CSessionManager::CContextWorkerHandler::handle(
    IContextWorker::TInputItems const &events)
{
    // events to repeat handling
    IContextWorker::TOutputItems events_to_repeat;

    for (auto const &worker_event: events) {
        
//        if (worker_event->status    == Event::TStatus::BEGIN &&
//            worker_event->operation == Event::TOperation::OPEN)
//        {
//            LOGT << "event: "
//                 << worker_event->operation << " "
//                 << worker_event->status    << " "
//                 << worker_event->stream->getURI();
//        } else {
//            LOGT << "event: "
//                 << worker_event->operation << " "
//                 << worker_event->status    << " "
//                 << worker_event->stream->getID();
//        }
        
        if (auto context = m_context_manager->acquireContext(worker_event)) {
            for (auto const &event: context->popEvents()) {
                
                switch (event->status) {
                    case Event::TStatus::BEGIN:
                        switch (event->operation) {
                            case Event::TOperation::OPEN:
                                event->stream->initialize();
                                m_multiplexer->subscribe(event->stream);
                                break;
                            case Event::TOperation::CLOSE:
                                m_multiplexer->unsubscribe(event->stream);
                                break;
                            case Event::TOperation::READ:
                            case Event::TOperation::WRITE:
                                if (context->transmit(event)) {
                                    LOGT << "transmit: true";
                                } else {
                                    event->operation = Event::TOperation::EOF_;
                                    LOGT << "transmit: false";
                                }
                                event->status = Event::TStatus::END;
                                events_to_repeat.push_back(event);

//                                // ----- for unix only
//                                event->status = Event::TStatus::END;
//                                events_to_repeat.push_back(event);
//                                // ----- for unix only

                                break;
                            default:
                                LOGT << "unhandled event: "
                                     << event->operation    << " "
                                     << event->status       << " "
                                     << event->stream->getURI();
                                event->status = Event::TStatus::END;
                                events_to_repeat.push_back(event);
                                break;
                        }
                        
                        break;
                        
                    case Event::TStatus::END:
                        if (context->update(event))
                            switch (event->operation) {
                                case Event::TOperation::OPEN:
                                    break;
                                case Event::TOperation::CLOSE:
                                    event->stream->finalize();
//                                    if (context->transmit(event))
//                                        events_to_repeat.push_back(event);
//                                    else
//                                        event->stream->finalize();
                                    break;
                                case Event::TOperation::READ:
                                case Event::TOperation::WRITE:
//                                    if (context->transmit(event)) {
//                                    event->status = Event::TStatus::BEGIN;
//                                    events_to_repeat.push_back(event);
//                                    }
                                    break;
                                case Event::TOperation::EOF_:
                                    break;
                                default:
                                    LOGT << "unhandled event: "
                                         << event->operation    << " "
                                         << event->status       << " "
                                         << event->stream->getURI();
                                    break;
                            }
                        else {
                            m_context_manager->removeContext(context);
                            continue;
                        }
                        
                        break;
                    default:
                        if (event->operation == Event::TOperation::TIMEOUT)
                            context->transmit(event);

                        if (!context->update(event))
                            m_context_manager->removeContext(context);
                };
            }
                
//            for (auto const &event: events_to_repeat)
//                LOGT << "repeat 1: "
//                     << event->operation    << " "
//                     << event->status       << " "
//                     << event->stream->getID();
            
            events_to_repeat.splice(events_to_repeat.end(), m_context_manager->releaseContext(context));
        }
    }
    
//    for (auto const &event: events_to_repeat)
//        LOGT << "repeat 2: "
//             << event->operation << " "
//             << event->status << " "
//             << event->stream->getID();
        
    return events_to_repeat; // ----->
}


} // implementation
} // io
} // iridium
