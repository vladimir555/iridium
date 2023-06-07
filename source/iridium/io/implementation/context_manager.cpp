#include "context_manager.h"

#include "context.h"


#include "iridium/logging/logger.h"
namespace iridium {
namespace io {
namespace implementation {


void CContextManager::createContext(IStream::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol) {
    LOCK_SCOPE();
    
    auto context = CContext::create(stream, protocol);
    m_map_stream_context[stream] = context;
    m_contexts.insert(context);
}


IContext::TSharedPtr CContextManager::acquireContext(Event::TSharedPtr const &event) {
    LOCK_SCOPE();

    if (event->operation == Event::TOperation::OPEN && event->status == Event::TStatus::BEGIN)
        LOGT << "manager::acquire: " << event->operation << " " << event->status << " " << event->stream->getURI();
    else
        LOGT << "manager::acquire: " << event->operation << " " << event->status << " " << event->stream->getID();
    
    auto stream_context  = m_map_stream_context.find(event->stream);
    if  (stream_context != m_map_stream_context.end()) {
        auto context = stream_context->second;
        context->pushEvent(event);
        auto i  = m_acquired_contexts.find(context);
        if  (i == m_acquired_contexts.end()) {
            m_acquired_contexts.insert(context);
            return context; // ----->
        }
    }
    // context not registered or acquired
    return nullptr; // ----->
}


std::list<Event::TSharedPtr> CContextManager::releaseContext(IContext::TSharedPtr const &context) {
    LOCK_SCOPE();
    
    m_acquired_contexts.erase(context);
    
    auto events = context->popEvents();
    
    if (m_contexts_to_remove.count(context) && events.empty()) {
        auto    stream_context  = m_map_stream_context.begin();
        while  (stream_context != m_map_stream_context.end()) {
            if (stream_context->second == context)
                stream_context  = m_map_stream_context.erase(stream_context);
            else
                stream_context++;
        }
        
        m_contexts.erase(context);
        m_contexts_to_remove.erase(context);
        LOGT << "manager::release, remove";
    }
    
    return events; // ----->
}


void CContextManager::removeContext(IContext::TSharedPtr const &context) {
    LOCK_SCOPE();
    
    LOGT << "manager::remove";
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


} // implementation
} // io
} // iridium
