#include "session_manager.h"

#include "multiplexer.h"

#include "iridium/logging/logger.h"
#include "iridium/threading/implementation/thread.h"
#include "iridium/threading/implementation/worker_pool.h"
#include "iridium/io/implementation/context_manager.h"
#include "iridium/items.h"


using iridium::threading::Synchronized;
using iridium::threading::implementation::CThread;
using iridium::threading::implementation::CWorkerPool;
using iridium::threading::implementation::CAsyncQueue;
using iridium::createObjects;


namespace iridium::io::implementation {


static std::list<Event::TSharedPtr> removeDuplicates(std::list<Event::TSharedPtr> const &events_) {
    if (events_.size() <= 1)
        return events_;

    std::vector<Event::TSharedPtr> filtered;
    filtered.reserve(events_.size());

    for (auto const &event: events_) {
        if (event && event->stream &&
          (!event->stream->getHandles().empty() ||
          ( event->operation    == Event::TOperation::OPEN &&
            event->status       == Event::TStatus::BEGIN)))
        {
            filtered.push_back(event);
        }
    }

    if (filtered.empty())
        return {};

    std::sort(filtered.begin(), filtered.end(),
        [] (auto const &a, auto const &b) {
            return
                std::tie(a->stream, a->operation, a->status) <
                std::tie(b->stream, b->operation, b->status);
        }
    );

    auto last = std::unique(filtered.begin(), filtered.end(),
        [] (auto const &a, auto const &b) {
            return
                std::tie(a->stream, a->operation, a->status) ==
                std::tie(b->stream, b->operation, b->status);
        }
    );

    filtered.erase(last, filtered.end());

    return {
        std::make_move_iterator(filtered.begin()),
        std::make_move_iterator(filtered.end())
    };
}


CSessionManager::CSessionManager()
:
    m_finalization_queue(
        CAsyncQueue<int>::create()),
    m_multiplexer(
        CMultiplexer::create()),
    m_context_manager(
        CContextManager::create()),
    m_context_worker(
        CWorkerPool<Event::TSharedPtr>::create(
            "context_handler",
            createObjects<IContextWorker::IHandler, CContextWorkerHandler>(
                std::thread::hardware_concurrency(), m_context_manager, m_multiplexer))),
    m_multiplexer_thread(
        CThread::create(
            "multiplexer",
            CMultiplexerThreadHandler::create(m_context_worker, m_context_manager, m_multiplexer)))
{}


void CSessionManager::initialize() {
    m_multiplexer->initialize();
    m_context_worker->initialize();
    m_multiplexer_thread->initialize();
}


void CSessionManager::finalize() {
    LOGT << "CSessionManager::finalize ...";
    m_multiplexer->finalize();
    m_context_worker->finalize();
    m_multiplexer_thread->finalize();
    LOGT << "CSessionManager::finalize OK";
}


void CSessionManager::manage(IStreamPort::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol) {
    // LOGT << "\n" << stream;
    if (stream && protocol) {
        m_context_manager->createContext(stream, protocol);
        auto event = Event::create(stream, Event::TOperation::OPEN, Event::TStatus::BEGIN);
        LOGT << "push: " << event;
        m_context_worker->push(event);
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
        auto events             = m_multiplexer->waitEvents();
        auto events_outdated    = m_context_manager->checkOutdatedStreams();

        events.insert(events.end(), events_outdated.begin(), events_outdated.end());

        m_context_worker->push(events);

        // if (!events.empty())
        //     LOGT << "multiplexer events:\n" << events;
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


CSessionManager::IContextWorker::IHandler::TOutputItems
CSessionManager::CContextWorkerHandler::handle(
    IContextWorker::IHandler::TInputItems const &events_)
{
    // LOGT << "handler events:" << events_;

    // if (events_.empty())
    //     return {}; // ----->

    // events to repeat handling
    IContextWorker::IHandler::TOutputItems events_to_repeat;

    // LOGT << "rm duplicates 1: " << events_;
    auto events = removeDuplicates(events_);
    // LOGT << "rm duplicates 2: " << events;

    for (auto const &worker_event: events) {
        // LOGT << "[WORKER] event: " << worker_event;
        if(!worker_event->stream ||
           (worker_event->stream->getHandles().empty() &&
            worker_event->operation != Event::TOperation::OPEN))
        {
            LOGT << "[SKIP]";
            continue; // <---
        }

        // events for multiple contexts
        if (auto context = m_context_manager->acquireContext(worker_event, m_multiplexer)) {
            bool is_context_valid = true;

            auto context_events = removeDuplicates(context->popEvents());

            // LOGT << "[CONTEXT] events: " << context_events;

            // events for one context
            for (auto const &event: /*removeDuplicates(context->popEvents())*/context_events) {
                // if (!is_context_valid)
                //     break; // --->

                LOGT << "context event: " << event;

                if (event->status == Event::TStatus::BEGIN) {
                    try {
                        if (event->operation == Event::TOperation::OPEN) {
                            LOGT << "[INIT]";
                            event->stream->initialize();
                            m_multiplexer->subscribe(event->stream);
                        }

                        else

                        if (event->operation == Event::TOperation::ERROR_) {
                            // redirect to protocol control
                            // LOGT << "[REDIRECT]: to protocol";
                            // event->status = Event::TStatus::END;
                            // events_to_repeat.push_back(event);
                            LOGT << "[UNSUBSCRIBE]";
                            m_multiplexer->unsubscribe(event->stream);
                            continue; // <---
                        }

                        else

                        if (event->operation == Event::TOperation::CLOSE) {
                            // read / write to end on close
                            LOGT << "[TRANSMIT]: flush";
                            while (context->transmit(event))
                                LOGT << "transmit flush next";
                            // event->status = Event::TStatus::END;
                            // events_to_repeat.push_back(event);
                            LOGT << "[UNSUBSCRIBE]";
                            m_multiplexer->unsubscribe(event->stream);
                        }

                        else

                        {
                            auto is_transmitted = context->transmit(event);
                            LOGT << "[TRANSMIT]: " << is_transmitted;

                            if (is_transmitted) {
                                event->status = Event::TStatus::END;
                                events_to_repeat.push_back(event);
                            }
                        }
                    } catch (std::exception const &e) {
                        LOGE
                            << "handling event error: " << e.what()
                            << "\n  event: " << event
                            << " " << event->stream->getURI();

                        event->operation    = Event::TOperation::ERROR_;
                        event->status       = Event::TStatus::END;

                        // to protocol controller
                        events_to_repeat.push_back(event);
                    }
                    continue; // <---
                }

                if (event->status == Event::TStatus::END) {
                    try {
                        try {
                            is_context_valid = context->update(event);
                        } catch (std::exception const &e) {
                            LOGE << "protocol error: " << e;
                            is_context_valid = false;
                        }

                        if (event->operation == Event::TOperation::CLOSE) {
                            LOGT << "[FINALIZE]";
                            event->stream->finalize();
                        }

                        else

                        if (is_context_valid &&
                            checkOneOf(
                                event->operation,
                                Event::TOperation::READ,
                                Event::TOperation::WRITE))
                        {
                            // repeat rw only if more data available
                            event->status = Event::TStatus::BEGIN;
                            events_to_repeat.push_back(event);
                            //LOGT << "repeat by context: " << event;
                            continue; // <---
                        } else {

                            // if (event->operation == Event::TOperation::CLOSE) {
                            //     event->stream->finalize();
                            //     event->status = Event::TStatus::END;
                            //     events_to_repeat.push_back(event);
                            // }

                            LOGT << "[SKIP]";
                        }
                    } catch (std::exception const &e) {
                        LOGE
                            << "handling event error: " << e.what()
                            << "\n  event: " << event
                            << " " << event->stream->getURI();
                        event->operation =  Event::TOperation::ERROR_;
                    }

                    // todo: client reconnect
                    if (event->operation == Event::TOperation::ERROR_)
                        is_context_valid = false;

                    continue; // <---
                }
            }

            auto events__ = m_context_manager->releaseContext(context, true/*is_context_valid*/);
            if (!events__.empty()) {
                events_to_repeat.insert(events_to_repeat.end(), events__.begin(), events__.end());
            }

        } else {
            // Context already acquired (occupied by another worker)
            // Put event back to the queue for retry
            if (worker_event->stream && !worker_event->stream->getHandles().empty()) {
                //LOGT << "context busy, retry: " << worker_event;
                events_to_repeat.push_back(worker_event);
            }
        }
    }

    events_to_repeat = removeDuplicates(events_to_repeat);

    //LOGT << "multiplexer wake events to repeat: " << events_to_repeat;

    m_multiplexer->wake(events_to_repeat);
    return {};
}


} // iridium::io::implementation
