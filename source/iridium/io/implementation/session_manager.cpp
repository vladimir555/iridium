#include "session_manager.h"

#include "multiplexer.h"

#include "iridium/logging/logger.h"
#include "iridium/threading/implementation/thread.h"
#include "iridium/threading/implementation/worker_pool.h"
#include "iridium/io/implementation/context_manager.h"
#include "iridium/items.h"

#include <unordered_map>
#include <unordered_set>


using iridium::threading::Synchronized;
using iridium::threading::implementation::CThread;
using iridium::threading::implementation::CWorkerPool;
using iridium::threading::implementation::CAsyncQueue;
using iridium::createObjects;


namespace iridium::io::implementation {


static std::list<Event::TSharedPtr> removeDuplicates(std::list<Event::TSharedPtr> const &events_) {
    if (events_.size() <= 1)
        return events_;

    struct EventKey {
        IStream::TSharedPtr stream;
        Event::TOperation   operation;
        Event::TStatus      status;

        bool operator == (EventKey const &other) const {
            return stream == other.stream && operation == other.operation && status == other.status;
        }
    };

    struct EventKeyHash {
        size_t operator () (EventKey const &k) const {
            size_t hash = std::hash<IStream::TSharedPtr>{}(k.stream);
            hash ^= std::hash<int>{}(static_cast<int>(k.operation)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= std::hash<int>{}(static_cast<int>(k.status))    + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
    };

    std::list<Event::TSharedPtr> filtered;
    std::unordered_set<EventKey, EventKeyHash> seen;

    for (auto const &event: events_) {
        if (event && event->stream &&
          (!event->stream->getHandles().empty() ||
            checkOneOf(
                event->operation,
                Event::TOperation::OPEN,
                Event::TOperation::CLOSE,
                Event::TOperation::ERROR_,
                Event::TOperation::TIMEOUT)))
        {
            EventKey key { event->stream, event->operation, event->status };
            if (seen.find(key) == seen.end()) {
                seen.insert(key);
                filtered.push_back(event);
            }
        }
    }

    return filtered;
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
        events = removeDuplicates(events);

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
    IContextWorker::IHandler::TOutputItems events_to_repeat;

    auto events = removeDuplicates(events_);

    std::vector<IStream::TSharedPtr> streams_order;
    std::unordered_map<IStream::TSharedPtr, std::list<Event::TSharedPtr>> stream_events;
    for (auto const &event : events) {
        if (event && event->stream) {
            if (stream_events.find(event->stream) == stream_events.end())
                streams_order.push_back(event->stream);
            stream_events[event->stream].push_back(event);
        }
    }

    for (auto const &stream : streams_order) {
        auto &events_batch = stream_events[stream];

        if (auto context = m_context_manager->acquireContext(events_batch.front(), m_multiplexer)) {
            bool is_context_valid = true;

            events_batch.pop_front();
            for (auto const &event : events_batch)
                context->pushEvent(event);

            auto context_events = removeDuplicates(context->popEvents());

            for (auto const &event: context_events) {
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
                            LOGT << "[UNSUBSCRIBE]";
                            m_multiplexer->unsubscribe(event->stream);
                            continue; // <---
                        }

                        else

                        if (event->operation == Event::TOperation::CLOSE) {
                            LOGT << "[TRANSMIT]: flush";
                            while (context->transmit(event))
                                LOGT << "transmit flush next";
                            LOGT << "[UNSUBSCRIBE]";
                            m_multiplexer->unsubscribe(event->stream);
                        }

                        else {
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
                            event->status = Event::TStatus::BEGIN;
                            events_to_repeat.push_back(event);
                            continue; // <---
                        } else {
                            LOGT << "[SKIP]";
                        }
                    } catch (std::exception const &e) {
                        LOGE
                            << "handling event error: " << e.what()
                            << "\n  event: " << event
                            << " " << event->stream->getURI();
                        event->operation =  Event::TOperation::ERROR_;
                    }

                    if (event->operation == Event::TOperation::ERROR_)
                        is_context_valid = false;

                    continue; // <---
                }
            }

            auto events__ = m_context_manager->releaseContext(context, is_context_valid);
            if (!events__.empty()) {
                events_to_repeat.insert(events_to_repeat.end(), events__.begin(), events__.end());
            }

        } else {
            // Context already acquired (occupied by another worker) or no context.
            // Push events to context. The context holder will re-queue them upon release.
            if (auto context_to_push = m_context_manager->getContext(stream)) {
                auto it = events_batch.begin();
                it++; // The first event was already pushed by acquireContext
                for (; it != events_batch.end(); ++it)
                    context_to_push->pushEvent(*it);
            } else {
                // No context yet? This shouldn't happen if acquireContext was called,
                // but just in case, re-queue.
                if (!stream->getHandles().empty() ||
                    checkOneOf(events_batch.front()->operation,
                               Event::TOperation::OPEN,
                               Event::TOperation::CLOSE,
                               Event::TOperation::ERROR_,
                               Event::TOperation::TIMEOUT))
                {
                    events_to_repeat.insert(events_to_repeat.end(), events_batch.begin(), events_batch.end());
                }
            }
        }
    }

    events_to_repeat = removeDuplicates(events_to_repeat);
    m_multiplexer->wake(events_to_repeat);
    return {};
}


} // iridium::io::implementation
