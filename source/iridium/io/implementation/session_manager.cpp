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
        if (event && event->stream)
            filtered.push_back(event);
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
    m_context_worker->initialize();
    m_multiplexer_thread->initialize();
}


void CSessionManager::finalize() {
    //LOGT << "CSessionManager::finalize ...";
    m_multiplexer_thread->finalize();
    m_context_worker->finalize();
    m_multiplexer->finalize();
    //LOGT << "CSessionManager::finalize OK";
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
        try {
            auto events             = m_multiplexer->waitEvents();
            auto events_outdated    = m_context_manager->checkOutdatedStreams();

            events.insert(events.end(), events_outdated.begin(), events_outdated.end());
            events = removeDuplicates(events);

            for (auto const &event: events)
                m_context_worker->push(event);

            if (!events.empty())
                ;//LOGT << "multiplexer events:\n" << events;
        } catch (std::exception const &e) {
            LOGE << "multiplexer thread error: " << e.what();
            threading::sleep(100);
        }
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
    //threading::sleep(1000);
    if (events_.empty())
        return {}; // ----->

    // events to repeat handling
    IContextWorker::IHandler::TOutputItems events_to_repeat;

    for (auto const &worker_event: removeDuplicates(events_)) {

        if (worker_event->stream->getHandles().empty() &&
            worker_event->operation != Event::TOperation::OPEN &&
            worker_event->operation != Event::TOperation::CLOSE &&
            worker_event->operation != Event::TOperation::ERROR_ &&
            worker_event->operation != Event::TOperation::TIMEOUT)
        {
            continue; // <---
        }

        // events for multiple contexts
        if (auto context = m_context_manager->acquireContext(worker_event, m_multiplexer)) {
            bool is_context_valid = true;

            auto context_events = removeDuplicates(context->popEvents());

            LOGT << "[WORKER] context events:" << context_events;

            // events for one context
            for (auto const &event: /*removeDuplicates(context->popEvents())*/context_events) {
                // if (!is_context_valid)
                //     break; // --->

                if (event->status == Event::TStatus::BEGIN) {
                    try {
                        if (event->operation == Event::TOperation::OPEN) {
                            event->stream->initialize();
                            m_multiplexer->subscribe(event->stream);
                        } else {
                            // Set operation flag and move to END for batched processing
                            context->setOperationFlag(event->operation);
                            event->status = Event::TStatus::END;
                            events_to_repeat.push_back(event);
                        }
                    } catch (std::exception const &e) {
                        LOGE
                            << "handling event error: " << e.what()
                            << "\n  event: " << event
                            << " " << event->stream->getURI();

                        // Convert to ERROR_ and let END handler process it
                        event->operation    = Event::TOperation::ERROR_;
                        event->status       = Event::TStatus::END;
                        events_to_repeat.push_back(event);
                    }
                }

                else

                if (event->status == Event::TStatus::END) {
                    try {
                        // OPEN only needs update, others use processOperationFlags
                        if (event->operation == Event::TOperation::OPEN) {
                            is_context_valid = context->update(event);
                        } else {
                            is_context_valid = context->processOperationFlags(event);
                        }
                    } catch (std::exception const &e) {
                        LOGE
                            << "handling event error: " << e.what()
                            << "\n  event:" << event;
                        is_context_valid = false;
                    }

                    if (event->operation == Event::TOperation::ERROR_)
                        is_context_valid = false;

                    // todo: client reconnect
                    if (event->operation == Event::TOperation::ERROR_)
                        is_context_valid = false;
                }
            } // for

            if (!is_context_valid)
                m_context_manager->removeContext(context);

            auto events__ = m_context_manager->releaseContext(context);
            events_to_repeat.insert(events_to_repeat.end(), events__.begin(), events__.end());

        } else {
            LOGT << "event without context: " << worker_event;
        }
    }

    events_to_repeat = removeDuplicates(events_to_repeat);

    //LOGT << "multiplexer wake events to repeat: " << events_to_repeat;

    m_multiplexer->wake(events_to_repeat);
    return {};
}


} // iridium::io::implementation
