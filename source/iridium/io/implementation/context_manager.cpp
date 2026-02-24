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


IContext::TSharedPtr CContextManager::getContext(IStream::TSharedPtr const &stream) {
    LOCK_SCOPE();
    auto i = m_map_stream_context.find(stream);
    return i != m_map_stream_context.end() ? i->second : nullptr;
}


IContext::TSharedPtr CContextManager::acquireContext(Event::TSharedPtr const &event, IMultiplexer::TSharedPtr const &multiplexer) {
    LOCK_SCOPE();

    auto stream_context  = m_map_stream_context.find(event->stream);
    if  (stream_context != m_map_stream_context.end()) {
        auto context = stream_context->second;
        context->pushEvent(event);
        auto i  = m_acquired_contexts.find(context);
        if  (i == m_acquired_contexts.end()) {
            m_acquired_contexts.insert(context);
            return context; // ----->
        } else {
            return nullptr; // ----->
        }
    }

    if (event->operation == Event::TOperation::CLOSE && event->status == Event::TStatus::BEGIN) {
        multiplexer->unsubscribe(event->stream);
    }

    return nullptr; // ----->
}


std::list<Event::TSharedPtr> CContextManager::releaseContext(IContext::TSharedPtr const &context, bool const &is_valid_context) {
    LOCK_SCOPE();

    m_acquired_contexts.erase(context);

    auto events = context->popEvents();

    if (events.empty() && !is_valid_context) {
        context->remove();

        auto    stream_context  = m_map_stream_context.begin();
        while  (stream_context != m_map_stream_context.end()) {
            if (stream_context->second == context)
                stream_context  = m_map_stream_context.erase(stream_context);
            else
                stream_context++;
        }

        m_contexts.erase(context);
        m_contexts_to_remove.erase(context);

        // LOGT << "CContextManager::releaseContext: empty";
        return {}; // ----->
    } else {
        return events; // ----->
    }
}


// void CContextManager::removeContext(IContext::TSharedPtr const &context) {
//     LOCK_SCOPE();
//     //LOGT << "CContextManager::removeContext: mark to remove on CLOSE END"; // rm on close end
//     if (m_contexts_to_remove.count(context) == 0) {
//         m_contexts_to_remove.insert(context);
//     }
// }


std::list<Event::TSharedPtr> CContextManager::checkOutdatedStreams() {
    LOCK_SCOPE();

    std::list<Event::TSharedPtr> result;
    for(auto const &context: m_contexts)
        result.splice(result.end(), context->checkOutdatedStreams());

    return result; // ----->
}


} // iridium::io::implementation
