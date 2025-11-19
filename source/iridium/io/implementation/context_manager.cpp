#include "context_manager.h"

#include "context.h"


#include "iridium/logging/logger.h"
namespace iridium::io::implementation {


void CContextManager::createContext(IStream::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol) {
    LOCK_SCOPE();

    auto context = CContext::create(stream, protocol);
    m_map_stream_context[stream] = context;
    m_contexts.insert(context);
}


IContext::TSharedPtr CContextManager::acquireContext(Event::TSharedPtr const &event, IMultiplexer::TSharedPtr const &multiplexer) {
    LOCK_SCOPE();

    //if (event->operation == Event::TOperation::OPEN && event->status == Event::TStatus::BEGIN)
    LOGT
        << "CContextManager::acquireContext begin: "
        << event->operation << " "
        << event->status << " "
        << event->stream->getHandles();
    //else
    //    LOGT << "manager::acquire: " << event->operation << " " << event->status << " " << event->stream->getID();

    auto stream_context  = m_map_stream_context.find(event->stream);
    if  (stream_context != m_map_stream_context.end()) {
        auto context = stream_context->second;
        context->pushEvent(event);
        auto i  = m_acquired_contexts.find(context);
        if  (i == m_acquired_contexts.end()) {
            m_acquired_contexts.insert(context);

            LOGT
                << "CContextManager::acquireContext end: "
                << event->stream->getHandles()
                << " return context";

            return context; // ----->
        } else {
            // context acquired
            LOGT
                << "CContextManager::acquireContext end: "
                << event->stream->getHandles()
                << " return nullptr, acquired";
            return nullptr; // ----->
        }
    }

//    // context not registered
//    if (event->operation == Event::TOperation::CLOSE && event->status == Event::TStatus::BEGIN) {
//        LOGT << "CContextManager::acquireContext, unsubscribe orphan event close begin: " << event->stream->getHandles();
//        multiplexer->unsubscribe(event->stream);
//    }

    if (event->operation == Event::TOperation::CLOSE) {
        if (event->status == Event::TStatus::BEGIN) {
            LOGT
                << "unsubscribe orphan close event: "
                << event->operation << " "
                << event->status << " "
                << event->stream->getHandles();
            multiplexer->unsubscribe(event->stream);
        }
        if (event->status == Event::TStatus::END) {
            LOGT << "finalize orphan stream: " << event->stream->getHandles();
            event->stream->finalize();
        }
    }

    LOGT
        << "CContextManager::acquireContext end: "
        << event->stream->getHandles()
        << " return nullptr";

    return nullptr; // ----->
}


std::list<Event::TSharedPtr> CContextManager::releaseContext(IContext::TSharedPtr const &context) {
//    // todo: throw
//    if (!context)
//        return {};

    auto events = context->popEvents();

    LOGT << "CContextManager::releaseContext, events count: " << events.size();

    LOCK_SCOPE();

    if (m_contexts_to_remove.count(context) > 0 && events.empty()) {
//        LOGT << "CContextManager::releaseContext, remove context";

        auto    stream_context  = m_map_stream_context.begin();
        while  (stream_context != m_map_stream_context.end()) {
            if (stream_context->second == context)
                stream_context  = m_map_stream_context.erase(stream_context);
            else
                stream_context++;
        }

        m_contexts.erase(context);
        m_contexts_to_remove.erase(context);

        LOGT << "CContextManager::releaseContext: empty";
        return {}; // ----->
    } else {
        m_acquired_contexts.erase(context);

        for (auto const &event: events) {
            LOGT
            << "CContextManager::releaseContext: "
            << event->stream->getHandles() << " "
            << event->operation << " "
            << event->status;
        }
        return events; // ----->
    }
}


void CContextManager::removeContext(IContext::TSharedPtr const &context) {
    LOCK_SCOPE();

    LOGT << "CContextManager::removeContext: mark to remove on CLOSE END"; // rm on close end
    if (m_contexts_to_remove.count(context) == 0) {
        context->remove();
        m_contexts_to_remove.insert(context);
    }
}


std::list<Event::TSharedPtr> CContextManager::checkOutdatedStreams() {
    LOCK_SCOPE();

    std::list<Event::TSharedPtr> result;
    for(auto const &context: m_contexts)
        result.splice(result.end(), context->checkOutdatedStreams());

    return result; // ----->
}


} // iridium::io::implementation
