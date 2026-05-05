#include "session_manager.h"

#include "multiplexer.h"
#include "platform/unix/multiplexer.h"

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

    // snapshot
    struct TEventView {
        Event::TSharedPtr
            ptr;
        void
           *stream_key;
        Event::TOperation
            operation;
        Event::TStatus
            status;

        bool operator<(TEventView const &rhs) const {
            return
                std::tie(    stream_key,     operation,     status) <
                std::tie(rhs.stream_key, rhs.operation, rhs.status);
        }
        bool operator==(TEventView const& rhs) const {
            return
                stream_key == rhs.stream_key &&
                operation == rhs.operation &&
                status == rhs.status;
        }
    };

    std::vector<TEventView> views;

    views.reserve(events_.size());

    for (auto const &event: events_) {
        if (!event || !event->stream) continue;

        bool is_keep =
            !event->stream->getHandles().empty() ||
            (event->operation == Event::TOperation::OPEN && event->status == Event::TStatus::BEGIN);

        if (is_keep) {
            // read once to avoid data racing
            views.push_back({
                event,
                event->stream.get(),
                event->operation,
                event->status
            });
        }
    }

    if (views.empty())
        return {};

    // sort and rm dups
    std::sort(views.begin(), views.end());
    auto last = std::unique(views.begin(), views.end(),
        [] (TEventView const &a, TEventView const &b) {
            return a == b;
        }
    );

    views.erase(last, views.end());

    std::list<Event::TSharedPtr> result;

    for (auto &v: views)
        result.push_back(std::move(v.ptr));

    return result;
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
    m_multiplexer->finalize();
    m_context_worker->finalize();
    m_multiplexer_thread->finalize();
}


void CSessionManager::manage(IStreamPort::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol) {
    // LOGT << "\n" << stream;
    if (stream && protocol) {
        m_context_manager->createContext(stream, protocol);
        auto event = Event::create(stream, Event::TOperation::OPEN, Event::TStatus::BEGIN);
        // LOGT << "push: " << event;
        m_context_worker->push(event);
    } else
        throw std::runtime_error("session manage error: null stream or protocol"); // ----->
}


CSessionManager::CMultiplexerThreadHandler::CMultiplexerThreadHandler(
    IContextWorker::TSharedPtr
        const &context_worker,
    IContextManager::TSharedPtr
        const &context_manager,
    IMultiplexer::TSharedPtr
        const &multiplexer)
:
    m_context_worker
        (context_worker),
    m_context_manager
        (context_manager),
    m_multiplexer
        (multiplexer)
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
    IContextManager::TSharedPtr
        const &context_manager,
    IMultiplexer::TSharedPtr
        const &multiplexer)
:
    m_context_manager
        (context_manager),
    m_multiplexer
        (multiplexer)
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
            // LOGT << "[SKIP]";
            continue; // <---
        }

        // events for multiple contexts
        if (auto context = m_context_manager->acquireContext(worker_event, m_multiplexer)) {
            bool is_context_valid = true;

            auto context_events = removeDuplicates(context->popEvents());

            //  LOGT << "[CONTEXT] events: " << context_events;

            // events for one context
            for (auto const &event: /*removeDuplicates(context->popEvents())*/context_events) {
                // if (!is_context_valid)
                //     break; // --->

                // threading::sleep(1000);
                //LOGT << "context event: " << event;

                if (event->status == Event::TStatus::BEGIN) {
                    try  {
                        if (event->operation == Event::TOperation::OPEN) {
                            if (event->stream->getHandles().empty()) {
                                // LOGT << "[INIT]";
                                event->stream->initialize();
                                // LOGT << "[SUBSCRIBE]";
                                m_multiplexer->subscribe(event->stream);
                            } else {
                                // LOGT << "[INIT SKIP]";
                            }
                        }

                        else

                        if (event->operation == Event::TOperation::ERROR_) {
                            // redirect to protocol control
                            // LOGT << "[REDIRECT]: to protocol";
                            // event->status = Event::TStatus::END;
                            // events_to_repeat.push_back(event);
                            // LOGT << "[UNSUBSCRIBE]";
                            m_multiplexer->unsubscribe(event->stream);
                            continue; // <---
                        }

                        else

                        if (event->operation == Event::TOperation::CLOSE) {
                            // read / write to end on close
                            auto pipe = context->getPipe(event);
                            if (pipe) {
                                // LOGT << "[TRANSMIT]: flush";

                                while (pipe->transmit(event));
                                    // LOGT << "transmit flush next";

                                // event->status = Event::TStatus::END;
                                // events_to_repeat.push_back(event);
                                // LOGT << "[UNSUBSCRIBE]";
                                m_multiplexer->unsubscribe(event->stream);
                            }
                        }

                        else

                        {
                            auto pipe = context->getPipe(event);
                            if (pipe) {
                                // LOGT << "[TRANSMIT]: found pipe";
                                // auto is_transmitted = false;
                                // while (pipe->transmit(event))
                                //     is_transmitted  = true;

                                auto is_transmitted = pipe->transmit(event);

                                // LOGT << "[TRANSMIT]: " << is_transmitted;

                                if (is_transmitted) {
                                    event->status = Event::TStatus::END;
                                    events_to_repeat.push_back(event);
                                }
                            } else {
                                // LOGT << "[TRANSMIT]: pipe not found";
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
                            LOGE << "protocol error: " << e << "\nevent:\n" << event;
                            is_context_valid = false;
                        }

                        if (event->operation == Event::TOperation::CLOSE) {
                            // LOGT << "[FINALIZE]";
                            if(!event->stream->getHandles().empty()) {
                                // todo: check empty -> not empty on interrupting process
                                try {
                                    event->stream->finalize();
                                } catch (...) {
                                }
                            }
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

                            // LOGT << "[SKIP]";
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
