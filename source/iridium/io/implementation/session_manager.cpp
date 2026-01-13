#include "session_manager.h"

#include "multiplexer.h"

#include "iridium/logging/logger.h"
#include "iridium/threading/implementation/thread.h"
#include "iridium/threading/implementation/worker_pool.h"
#include "iridium/io/implementation/context_manager.h"
#include "iridium/items.h"

#include <set>


using iridium::threading::Synchronized;
using iridium::threading::implementation::CThread;
using iridium::threading::implementation::CWorkerPool;
using iridium::threading::implementation::CAsyncQueue;
using iridium::createObjects;


namespace iridium::io::implementation {


static std::list<Event::TSharedPtr> removeDuplicates(std::list<Event::TSharedPtr> const &events_) {
    struct TComparator {
        bool operator()(
            Event::TSharedPtr const &left,
            Event::TSharedPtr const &right) const
        {
            return
                left            &&              right                       &&
                left->stream    &&              right->stream               &&
               (left->stream->getHandles()  <   right->stream->getHandles() ||
               (left->stream->getHandles()  ==  right->stream->getHandles() &&
               (left->operation             <   right->operation ||
               (left->operation             ==  right->operation &&
                left->status                <   right->status))));
        }
    };

    std::set<Event::TSharedPtr, TComparator> events;
    for (auto const &event: events_)
        events.insert(event);

    return std::list<Event::TSharedPtr>(events.begin(), events.end()); // ----->
}


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
            CMultiplexerThreadHandler::create(m_context_worker, m_context_manager, m_multiplexer)))
{}


void CSessionManager::initialize() {
    m_multiplexer->initialize();
    m_multiplexer_thread->initialize();
    m_context_worker->initialize();
}


void CSessionManager::finalize() {
    LOGT << "CSessionManager::finalize ...";
    m_context_worker->finalize();
    m_multiplexer->finalize();
    m_multiplexer_thread->finalize();
    LOGT << "CSessionManager::finalize OK";
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


void CSessionManager::CMultiplexerThreadHandler::finalize() {
    LOGT << "CMultiplexerThreadHandler::finalize";
}


void CSessionManager::CMultiplexerThreadHandler::run(std::atomic<bool> &is_running) {
    while (is_running) {
        auto events = removeDuplicates(m_multiplexer->waitEvents());

        LOGT << "multiplexer events:\n" << events;

        m_context_worker->push(events);
        m_context_worker->push(m_context_manager->checkOutdatedStreams());
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


void CSessionManager::CContextWorkerHandler::finalize() {
    LOGT << "CContextWorkerHandler::finalize";
}


CSessionManager::IContextWorker::IHandler::TOutputItems
CSessionManager::CContextWorkerHandler::handle(
    IContextWorker::IHandler::TInputItems const &events_)
{
    //threading::sleep(1000);
    if (events_.empty())
        return {}; // ----->

    // events to repeat handling
    IContextWorker::IHandler::TOutputItems events_to_repeat;

    for (auto const &worker_event: removeDuplicates(events_)) {

        // LOGT << "handle event (worker):   " << worker_event;

        // events for multiple contexts
        if (auto context = m_context_manager->acquireContext(worker_event, m_multiplexer)) {
            bool is_context_valid = true;

            // events for one context
            for (auto const &event: removeDuplicates(context->popEvents())) {
                // if (!is_context_valid)
                //     break; // --->

                if (event->status == Event::TStatus::BEGIN) {
                    try {
                        if (event->operation == Event::TOperation::OPEN) {
                            event->stream->initialize();
                            m_multiplexer->subscribe(event->stream);
                        } else
                        if (checkOneOf(
                            event->operation,
                            Event::TOperation::READ,
                            Event::TOperation::WRITE,
                            Event::TOperation::TIMEOUT,
                            Event::TOperation::CLOSE))
                        {
                            auto is_transmitted = context->transmit(event);

                            if (is_transmitted || event->operation == Event::TOperation::CLOSE) {
                                event->status = Event::TStatus::END;
                                events_to_repeat.push_back(event);
                            }
                        }
                    } catch (std::exception const &e) {
                        LOGE
                            << event
                            << "\n" << e.what()
                            << "\n" << event->stream->getURI();

                        event->operation    = Event::TOperation::ERROR_;
                        event->status       = Event::TStatus::END;

                        events_to_repeat.push_back(event);
                    }
                    continue; // <---
                }

                if (event->status == Event::TStatus::END) {
                    try {
                        is_context_valid = context->update(event);

                        if (is_context_valid &&
                            checkOneOf(
                                event->operation,
                                Event::TOperation::READ,
                                Event::TOperation::WRITE,
                                Event::TOperation::CLOSE))
                        {
                            // repeat rw
                            event->status = Event::TStatus::BEGIN;
                            events_to_repeat.push_back(event);
                            LOGT << "repeat by context: " << event;
                            continue; // <---
                        } else {
                            LOGT << "skip: " << event;
                        }

                        if (event->operation == Event::TOperation::CLOSE) {
                            LOGT << "finalize on close end: " << event;
                            event->stream->finalize();
                            m_multiplexer->unsubscribe(event->stream);
                        }
                    } catch (std::exception const &e) {
                        LOGE
                            << event
                            << "\n" << e.what()
                            << "\n" << event->stream->getURI();
                        event->operation =  Event::TOperation::ERROR_;
                    }

                    if (event->operation == Event::TOperation::ERROR_)
                        is_context_valid = false;

                    continue; // <---
                }
            }

            if (is_context_valid) {
                auto events__ = m_context_manager->releaseContext(context);
                events_to_repeat.insert(events_to_repeat.end(), events__.begin(), events__.end());
            } else {
                LOGT << "remove context";
//                context->popEvents();
                m_context_manager->removeContext(context);
            }

        } else {
//            // finalize orphans
//            if (worker_event->stream && !worker_event->stream->getHandles().empty()) {
//                LOGW << "orphan event, unsubscribing: " << worker_event;
//                m_multiplexer->unsubscribe(worker_event->stream);
//            }
        }
    }

    m_multiplexer->wake(events_to_repeat);
    return {};
}


} // iridium::io::implementation
