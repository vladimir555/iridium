#include "session_manager.h"

#include "multiplexer.h"

#include "iridium/logging/logger.h"
#include "iridium/threading/implementation/thread.h"
#include "iridium/threading/implementation/worker_pool.h"
#include "iridium/io/implementation/context_manager.h"
#include "iridium/items.h"

#include <set>
#include <tuple>


using iridium::threading::Synchronized;
using iridium::threading::implementation::CThread;
using iridium::threading::implementation::CWorkerPool;
using iridium::threading::implementation::CAsyncQueue;
using iridium::createObjects;


namespace iridium::io::implementation {


static std::list<Event::TSharedPtr> removeDuplicates(std::list<Event::TSharedPtr> const &events_) {
    if (events_.size() <= 1)
        return events_;

    std::list<Event::TSharedPtr> result;
    std::set<std::tuple<IStream*, Event::TOperation, Event::TStatus>> seen;

    for (auto const &event: events_) {
        if (event && event->stream) {
            auto key = std::make_tuple(event->stream.get(), event->operation, event->status);
            if (seen.insert(key).second) {
                result.push_back(event);
            }
        }
    }

    return result;
}


CSessionManager::CSessionManager()
:
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
    if (m_multiplexer)
        m_multiplexer->finalize();
    if (m_multiplexer_thread)
        m_multiplexer_thread->finalize();
    if (m_context_worker)
        m_context_worker->finalize();
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


void CSessionManager::CMultiplexerThreadHandler::finalize() {}


void CSessionManager::CMultiplexerThreadHandler::run(std::atomic<bool> &is_running) {
    while (is_running) {
        auto events             = m_multiplexer->waitEvents();
        auto events_outdated    = m_context_manager->checkOutdatedStreams();

        events.insert(events.end(), events_outdated.begin(), events_outdated.end());
        events = removeDuplicates(events);

        m_context_worker->push(events);

        if (!events.empty())
            LOGT << "multiplexer events:\n" << events;
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

    // Group events by stream to avoid redundant acquire/release calls and double-processing
    std::unordered_map<IStream::TSharedPtr, std::list<Event::TSharedPtr>> grouped_events;
    for (auto const &event : removeDuplicates(events_)) {
        if (event->stream)
            grouped_events[event->stream].push_back(event);
    }

    for (auto const &pair : grouped_events) {
        auto const &stream = pair.first;
        auto const &batch_events = pair.second;

        // Check if at least one event in the batch for this stream is valid for processing
        bool has_valid_event = false;
        for (auto const &event : batch_events) {
            if (!(stream->getHandles().empty() &&
                  !checkOneOf(event->operation,
                      Event::TOperation::OPEN,
                      Event::TOperation::CLOSE,
                      Event::TOperation::ERROR_,
                      Event::TOperation::TIMEOUT)))
            {
                has_valid_event = true;
                break;
            }
        }

        if (!has_valid_event)
            continue;

        // Use the first event to acquire context
        auto it = batch_events.begin();
        if (auto context = m_context_manager->acquireContext(*it, m_multiplexer)) {

            // Push any other events from this same batch to the context's internal queue
            for (++it; it != batch_events.end(); ++it) {
                context->pushEvent(*it);
            }

            bool is_context_valid = true;

            auto context_events = removeDuplicates(context->popEvents());

            // LOGT << "context_events: " << context_events;

            // events for one context
            for (auto const &event: context_events) {
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

            auto events__ = m_context_manager->releaseContext(context, is_context_valid);
            if (!events__.empty()) {
                events_to_repeat.insert(events_to_repeat.end(), events__.begin(), events__.end());
            }

        } else {
            // Context already acquired (occupied by another worker) or missing.
            // We already pushed the FIRST event (*it) in acquireContext call.
            // We MUST push the rest too, so the current owner can see them.
            if (auto ctx = m_context_manager->getContext(stream)) {
                for (++it; it != batch_events.end(); ++it) {
                    ctx->pushEvent(*it);
                }
            }
        }
    }

    events_to_repeat = removeDuplicates(events_to_repeat);

    //LOGT << "multiplexer wake events to repeat: " << events_to_repeat;

    m_multiplexer->wake(events_to_repeat);
    return {};
}


} // iridium::io::implementation
