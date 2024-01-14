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


std::list<Event::TSharedPtr> removeDuplicates(std::list<Event::TSharedPtr> const &events_) {
    std::unordered_map<size_t, Event::TSharedPtr>   map_hash_event;
    std::hash<Event::TSharedPtr>                    hash;
    std::list<Event::TSharedPtr>                    events;

    for (auto const &event : events_) {
        if (event->operation == Event::TOperation::OPEN)
            events.push_back(event);
        else
            map_hash_event[hash(event)] = event;
    }

    for (auto const &i : map_hash_event)
        events.push_back(i.second);

    return events; // ----->
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
        //LOGT << "multiplexer wait ...";
        
        auto    events = m_multiplexer->waitEvents();
        
        //LOGT << "multiplexer wait OK, events: " << events.size();
//        m_context_worker->push(m_multiplexer->waitEvents());
        
        string  events_str;
        for (auto const &event: events)
            events_str +=
                    convert<string>(event->operation)
            + " " + convert<string>(event->status)
            + " " + convert<string>(event->stream->getHandles().front())
            + "\n";
        
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
    IContextWorker::TInputItems const &events_)
{
    //threading::sleep(1000);
    if (events_.empty())
        return {}; // ----->
    //{
    //    string events_str;
    //    for (auto const &event : events_)
    //        events_str +=
    //            "\n"  + convert<string>(std::hash<Event::TSharedPtr>()(event))
    //            + " " + convert<string>(event->operation)
    //            + " " + convert<string>(event->status)
    //            + " " + (event->stream->getHandles().empty() ? "" :
    //                    convert<string>(event->stream->getHandles().front()));
    //    LOGT << "handle events 1:" << events_str;
    //}

    std::list<Event::TSharedPtr> events = removeDuplicates(events_);

    {
        string events_str;
        for (auto const &event: events)
            events_str +=
                 "\n" + convert<string>(std::hash<Event::TSharedPtr>()(event))
                + " " + (event->stream->getHandles().empty() ? "" :
                        convert<string>(event->stream->getHandles().front()))
                + " " + convert<string>(event->operation)
                + " " + convert<string>(event->status);
        //if (!events_str.empty())
        //    LOGT << "handle events:" << events_str;
    }

    // events to repeat handling
    IContextWorker::TOutputItems events_to_repeat;

    for (auto const &worker_event: events) {
        if (auto context = m_context_manager->acquireContext(worker_event)) {
            bool is_context_valid = true;
            for (auto const &event: removeDuplicates(context->popEvents())) {
                //LOGT
                //    << "handle event: "
                //    <<(event->stream->getHandles().empty() ? 0 :
                //       event->stream->getHandles().front()) << " "
                //    << event->operation << " "
                //    << event->status;

                if (event->status == Event::TStatus::BEGIN) {
                    try {
                        if (event->operation == Event::TOperation::OPEN) {
                            event->stream->initialize();
                            m_multiplexer->subscribe(event->stream);
                        } else
                        if (event->operation == Event::TOperation::CLOSE) {
                            m_multiplexer->unsubscribe(event->stream);
                            event->stream->finalize();
                        } else
                        if (checkOneOf(
                            event->operation, 
                            Event::TOperation::READ,
                            Event::TOperation::WRITE,
                            Event::TOperation::TIMEOUT))
                        {
                            auto is_transmitted = context->transmit(event);
                            //LOGT << "transmit: " << is_transmitted;
                            if (!is_transmitted) {
                                //LOGT << "eof by transmitter";
                                event->operation = Event::TOperation::EOF_;
                            }
                            event->status = Event::TStatus::END;
//#ifndef WINDOWS_PLATFORM
                            events_to_repeat.push_back(event);
//#endif // !WINDOWS_PLATFORM
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
                            event->status = Event::TStatus::BEGIN;
                            events_to_repeat.push_back(event);
                            //LOGT
                            //    << "repeat by context: "
                            //    << event->stream->getHandles().front() << " "
                            //    << event->operation << " "
                            //    << event->status;
                        } else {

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
                //LOGE << "remove context"; 
                m_context_manager->removeContext(context);
                continue; // <---
            }
            auto events_ = m_context_manager->releaseContext(context);
            events_to_repeat.insert(events_to_repeat.end(), events_.begin(), events_.end());
        }
    }

    {
        string events_str;
        for (auto const &event : events_to_repeat)
            events_str +=
                 "\n" + convert<string>(std::hash<Event::TSharedPtr>()(event))
                + " " + (event->stream->getHandles().empty() ? "" :
                        convert<string>(event->stream->getHandles().front()))
                + " " + convert<string>(event->operation)
                + " " + convert<string>(event->status);
        //if (!events_str.empty())
        //    LOGT << "repeat events:" << events_str;
    }

    return events_to_repeat; // ----->
}

//                switch (event->status) {
//                    case Event::TStatus::BEGIN:
//                        try {
//                            switch (event->operation) {
//                                case Event::TOperation::OPEN:
//                                    event->stream->initialize();
//                                    m_multiplexer->subscribe(event->stream);
//#ifdef WINDOWS_PLATFORM
//                                    event->status = Event::TStatus::END;
//                                    events_to_repeat.push_back(event);
//#endif // WINDOWS_PLATFORM
//                                    break; // --->
//                                case Event::TOperation::CLOSE:
//                                    m_multiplexer->unsubscribe(event->stream);
//                                    break; // --->
//                                case Event::TOperation::READ:
//                                case Event::TOperation::WRITE:
//                                    if (context->transmit(event)) {
//                                        LOGT << "transmit: true";
//                                    } else {
//                                        LOGT << "transmit: false";
//                                        event->operation    = Event::TOperation::EOF_;
//                                        event->status       = Event::TStatus::END;
//                                    }
//                                
//                                    events_to_repeat.push_back(event);
//
//    //                                // ----- for unix only
//    //                                event->status = Event::TStatus::END;
//    //                                events_to_repeat.push_back(event);
//    //                                // ----- for unix only
//
//                                    break; // --->
//                                case Event::TOperation::EOF_:
//                                    if (context->transmit(event))
//                                        events_to_repeat.push_back(event);
//                                    else
//                                        event->status = Event::TStatus::END;
//                                    break; // --->
//                                case Event::TOperation::ERROR_:
//                                    event->status = Event::TStatus::END;
//                                    break; // --->
//                                default:
//                                    LOGT << "unhandled event: "
//                                         << event->operation    << " "
//                                         << event->status       << " "
//                                         << event->stream->getURI();
//                                    event->status = Event::TStatus::END;
//                                    events_to_repeat.push_back(event);
//                                    break; // --->
//                            }
//                        } catch (std::exception const &e) {
//                            LOGE << e.what();
//                            event->operation = Event::TOperation::ERROR_;
//                        }
//                        
//                        break; // --->
//                    
//                    case Event::TStatus::END:
//                        if (context->update(event)) {
//                            LOGT << "update: true";
//                            try {
//                                switch (event->operation) {
//                                    case Event::TOperation::OPEN:
//#ifdef WINDOWS_PLATFORM
//                                        auto is_transmitted = context->transmit(event);
//                                        LOGT << "transmit: " << is_transmitted;
//#endif // WINDOWS_PLATFORM
//                                        break; // --->
//                                    case Event::TOperation::CLOSE:
//                                        event->stream->finalize();
//                                        break; // --->
//                                    case Event::TOperation::READ:
//                                    case Event::TOperation::WRITE:
//                                        event->status = Event::TStatus::BEGIN;
//                                        events_to_repeat.push_back(event);
//                                        break; // --->
//                                    case Event::TOperation::EOF_:
//                                        break; // --->
//                                    default:
//                                        LOGT << "unhandled event: "
//                                             << event->operation    << " "
//                                             << event->status       << " "
//                                             << event->stream->getURI();
//                                        break; // --->
//                                }
//                            } catch (std::exception const &e) {
//                                LOGE << e.what();
//                                event->operation = Event::TOperation::ERROR_;
//                            }
//                        } else {
//                            LOGT << "update: false; rm context";
//                            m_context_manager->removeContext(context);
//                            continue; // <---
//                        }
//                        
//                        break; // --->
//                    default:
//                        if (event->operation == Event::TOperation::TIMEOUT)
//                            context->transmit(event);
//
//                        if (!context->update(event))
//                            m_context_manager->removeContext(context);
//                };
                
            //for (auto const &event: events_to_repeat)
            //    LOGT << "repeat 1: "
            //         << event->operation    << " "
            //         << event->status       << " "
            //         << event->stream->getURI();
            


} // implementation
} // io
} // iridium
