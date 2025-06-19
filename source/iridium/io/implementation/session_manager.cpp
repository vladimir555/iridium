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


namespace iridium {
namespace io {
namespace implementation {


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

//    {
//        for (auto const &event: events_)
//            LOGT 
//                << "before: "
//                << (event->stream->getHandles().empty() ? 0: event->stream->getHandles().front()) << " "
//                << event->operation << " "
//                << event->status;
//    }
//
//    {
//        for (auto const &event: events)
//            LOGT
//                << "after:  "
//                << (event->stream->getHandles().empty() ? 0: event->stream->getHandles().front()) << " "
//                << event->operation << " "
//                << event->status;
//    }

//    {
//        string events_str;
//        for (auto const &event: events_)
//            events_str +=
//                 "\n" + (event->stream->getHandles().empty() ? "" :
//                        convert<string>(event->stream->getHandles().front()))
//                + " " + convert<string>(event->operation)
//                + " " + convert<string>(event->status);
//        if (!events_str.empty())
//            LOGT << "before:" << events_str;
//    }
//
//    {
//        string events_str;
//        for (auto const &event: events)
//            events_str +=
//                 "\n" + (event->stream->getHandles().empty() ? "" :
//                        convert<string>(event->stream->getHandles().front()))
//                + " " + convert<string>(event->operation)
//                + " " + convert<string>(event->status);
//        if (!events_str.empty())
//            LOGT << "after:" << events_str;
//    }

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
    LOGT << "CSessionManager::finalize ...";
    m_context_worker->finalize();
    m_event_repeater_thread->finalize();
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
        //LOGT << "multiplexer wait ...";
        
        auto    events = m_multiplexer->waitEvents();
        
        //LOGT << "multiplexer wait OK, events: " << events.size();
//        m_context_worker->push(m_multiplexer->waitEvents());
        
//        string  events_str;
//        for (auto const &event: events)
//            events_str +=
//                    convert<string>(event->operation)
//            + " " + convert<string>(event->status)
//            + " " + convert<string>(event->stream->getHandles().front())
//            + "\n";
        
        //LOGT << "multiplexer events:\n" + events_str;

        m_context_worker->push(events);
        m_context_worker->push(m_context_manager->checkOutdatedStreams());
    }
}


CSessionManager::CEventRepeaterHandler::CEventRepeaterHandler(IContextWorker::TSharedPtr const &context_worker)
:
    m_context_worker(context_worker)
{}


void CSessionManager::CEventRepeaterHandler::initialize() {}


void CSessionManager::CEventRepeaterHandler::finalize() {
    LOGT << "CEventRepeaterHandler::finalize";
}


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

//    std::list<Event::TSharedPtr> events = removeDuplicates(events_);
//    std::list<Event::TSharedPtr> events = events_; // = removeDuplicates(events_);

// todo: rm logs
//    {
//        string events_str;
//        for (auto const &event: events)
//            events_str +=
//                 "\n" + convert<string>(std::hash<Event::TSharedPtr>()(event))
//                + " " + (event->stream->getHandles().empty() ? "" :
//                        convert<string>(event->stream->getHandles().front()))
//                + " " + convert<string>(event->operation)
//                + " " + convert<string>(event->status);
//        if (!events_str.empty())
//            LOGT << "handle events:" << events_str;
//    }

    // events to repeat handling
    IContextWorker::IHandler::TOutputItems events_to_repeat;

    for (auto const &worker_event: removeDuplicates(events_)) {

        LOGT
            << "handle event (worker):   "
            << worker_event->stream->getHandles() << " "
            << worker_event->operation << " "
            << worker_event->status;

//        if (worker_event->operation == Event::TOperation::CLOSE &&
//            worker_event->status    == Event::TStatus::BEGIN    &&
//           !worker_event->stream->getHandles().empty()) 
//        {
//            m_multiplexer->unsubscribe(worker_event->stream);
//            worker_event->stream->finalize();
//            worker_event->status = Event::TStatus::END;
//            events_to_repeat.push_back(worker_event);
//            continue; // <---
//        }

        if (auto context = m_context_manager->acquireContext(worker_event, m_multiplexer)) {
            bool is_context_valid = true;

            for (auto const &event: removeDuplicates(context->popEvents())) {

                LOGT
                    << "handle event (acquired): "
                    << event->stream->getHandles() << " "
                    << event->operation << " "
                    << event->status;

                if (event->stream->getHandles().empty() &&
                    event->operation != Event::TOperation::OPEN)
                {
                    LOGT << "skip empty fd: " << event->operation;
                    continue;
                }

                if (event->stream->getHandles().empty() && !checkOneOf(
                    event->operation,
                    Event::TOperation::OPEN,
                    Event::TOperation::TIMEOUT))
                    continue; // <---

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

                            if (event->operation == Event::TOperation::CLOSE) {
                                LOGT << "unsubscribe on close begin";
                                m_multiplexer->unsubscribe(event->stream);
                                is_context_valid = false;
                            } else {
                                if (!is_transmitted)
                                    event->operation = Event::TOperation::EOF_;
                            }

                            event->status = Event::TStatus::END;
                            events_to_repeat.push_back(event);
                        }

                    } catch (std::exception const &e) {
                        LOGE
                            << event->operation << " " 
                            << event->status
                            << "\n" << e.what()
                            << "\n" << event->stream->getURI();
                        event->operation = Event::TOperation::ERROR_;
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
                                Event::TOperation::WRITE)) 
                        {
                            // repeat rw
                            event->status = Event::TStatus::BEGIN;
                            events_to_repeat.push_back(event);
                            LOGT
                                << "repeat by context: "
                                << event->stream->getHandles() << " "
                                << event->operation << " "
                                << event->status;
                        } else {

                        }
                        if (event->operation == Event::TOperation::CLOSE) {
                            LOGT
                                << "finalize on close end: "
                                << event->stream->getHandles() << " "
                                << event->operation << " "
                                << event->status;

                            LOGT << "finalize stream: " << event->stream->getHandles();
                            event->stream->finalize();
//                            LOGT
//                                << "unsubscribe on close end: "
//                                << event->stream->getHandles() << " "
//                                << event->operation << " "
//                                << event->status;
//                            m_multiplexer->unsubscribe(event->stream);
                        }
                        //if (event->operation == Event::TOperation::CLOSE) {
                        //    // todo: rm stream from context
                        //}
                    } catch (std::exception const &e) {
                        LOGE 
                            << event->operation << " " 
                            << event->status
                            << "\n" << e.what()
                            << "\n" << event->stream->getURI();
                        event->operation =  Event::TOperation::ERROR_;
                    }
                    if (event->operation == Event::TOperation::ERROR_)
                        is_context_valid = false;

                    continue; // <---
                }
            }
            if (!is_context_valid) {
                LOGT << "remove context"; 
                m_context_manager->removeContext(context);
//                continue; // <---
            }

            auto events_ = m_context_manager->releaseContext(context);
            events_to_repeat.insert(events_to_repeat.end(), events_.begin(), events_.end());
        } else {
//            if (worker_event->operation == Event::TOperation::CLOSE) {
//                if (worker_event->status == Event::TStatus::BEGIN) {
//                    LOGT
//                        << "unsubscribe orphan close event: "
//                        << worker_event->operation << " "
//                        << worker_event->status << " "
//                        << worker_event->stream->getHandles();
//                    m_multiplexer->unsubscribe(worker_event->stream);
//                }
//                if (worker_event->status == Event::TStatus::END) {
//                    LOGT << "finalize orphan stream: " << worker_event->stream->getHandles();
//                    worker_event->stream->finalize();
//                }
//            }
        }
    }

//    {
//        string events_str;
//        for (auto const &event : events_to_repeat)
//            events_str +=
//                 "\n" + convert<string>(std::hash<Event::TSharedPtr>()(event))
//                + " " + (event->stream->getHandles().empty() ? "" :
//                        convert<string>(event->stream->getHandles().front()))
//                + " " + convert<string>(event->operation)
//                + " " + convert<string>(event->status);
//        //if (!events_str.empty())
//        //    LOGT << "repeat events:" << events_str;
//    }

    return events_to_repeat; // ----->
}


} // implementation
} // io
} // iridium
