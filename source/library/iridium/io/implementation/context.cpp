#include "context.h"

#include "iridium/convertion/convert.h"
#include "iridium/assert.h"


using iridium::convertion::convert;
using std::string;


#include "iridium/logging/logger.h"
namespace iridium::io::implementation {


// void CContext::setPipe(
//     std::string
//         const &name,
//     URI::TSharedPtr
//         const &reader_uri,
//     URI::TSharedPtr
//         const &writer_uri)
// {
//     LOGT << "setPipe actions 1: " << m_actions;
//     LOGT << "setPipe name: " << name << ", reader_uri: " << reader_uri << ", writer_uri: " << writer_uri;
//     auto pipe = getPipe(name);

//     if (pipe && pipe->reader && pipe->reader->uri && pipe->reader->uri != reader_uri) {
//         m_actions.push_back( TAction { pipe->reader->uri, TStreamType::READER, TActionType::UNSUBSCRIBE } );
//         if (pipe->reader->uri != writer_uri)
//             m_actions.push_back( TAction { pipe->reader->uri, TStreamType::READER, TActionType::CLOSE } );
//         LOGT << "m_map_uri_pipe.erase reader " << pipe->reader->uri;
//         m_map_uri_pipe.erase(pipe->reader->uri);
//     }

//     if (pipe && pipe->writer && pipe->writer->uri && pipe->writer->uri != writer_uri) {
//         m_actions.push_back( TAction { pipe->writer->uri, TStreamType::WRITER, TActionType::UNSUBSCRIBE } );
//         if (pipe->writer->uri != reader_uri)
//             m_actions.push_back( TAction { pipe->writer->uri, TStreamType::WRITER, TActionType::CLOSE } );
//         LOGT << "m_map_uri_pipe.erase writer " << pipe->writer->uri;
//         m_map_uri_pipe.erase(pipe->writer->uri);
//     }

//     if(!pipe && (reader_uri || writer_uri)) {
//         pipe            = TPipe::create();
//         pipe->reader    = TStream::create();
//         pipe->writer    = TStream::create();
//         pipe->name      = name;

//         if (reader_uri)
//             m_actions.push_back( TAction { reader_uri, TStreamType::READER, TActionType::OPEN } );
//         if (writer_uri)
//             m_actions.push_back( TAction { writer_uri, TStreamType::WRITER, TActionType::OPEN } );
//     }

//     if (pipe && pipe->reader->uri != reader_uri) {
//         pipe->reader->uri = reader_uri;
//         if (reader_uri) {
//             m_actions.push_back( TAction { pipe->reader->uri, TStreamType::READER, TActionType::SUBSCRIBE } );
//             m_map_uri_pipe[pipe->reader->uri] = pipe;
//         }
//     }

//     if (pipe && pipe->writer->uri != writer_uri) {
//         pipe->writer->uri = writer_uri;
//         if (writer_uri) {
//             m_actions.push_back( TAction { pipe->writer->uri, TStreamType::WRITER, TActionType::SUBSCRIBE } );
//             m_map_uri_pipe[pipe->writer->uri] = pipe;
//         }
//     }

//     if (pipe)
//         m_map_name_pipe[name] = pipe;
//     else
//         m_map_name_pipe.erase(name);

//     LOGT << "setPipe actions 2: " << m_actions;

//     LOGT << "map_uri_pipe 11, " << reinterpret_cast<uint64_t>(this);
//     for (auto const &uri_pipe: m_map_uri_pipe) {
//         LOGT
//         << "map_uri_pipe: " << uri_pipe.first
//         << " " << uri_pipe.second->name
//         << " " << uri_pipe.second->reader->uri
//         << " " << uri_pipe.second->writer->uri;
//     }
//     LOGT << "map_uri_pipe 22";
// }


void CContext::setPipe(
    std::string const &name,
    URI::TSharedPtr reader_uri,
    URI::TSharedPtr writer_uri)
{
    LOGT << "setPipe actions 1: " << m_actions;
    LOGT << "setPipe name: " << name << ", reader_uri: " << reader_uri << ", writer_uri: " << writer_uri;
    auto pipe = getPipe(name);

    // === ЛОГ: состояние pipe после getPipe ===
    if (pipe) {
        LOGT << "setPipe getPipe: reader->uri=" << pipe->reader->uri
             << ", writer->uri=" << pipe->writer->uri;
    }

    if (pipe && pipe->reader && pipe->reader->uri && pipe->reader->uri != reader_uri) {
        m_actions.push_back(TAction{pipe->reader->uri, TStreamType::READER, TActionType::UNSUBSCRIBE});
        if (pipe->reader->uri != writer_uri)
            m_actions.push_back(TAction{pipe->reader->uri, TStreamType::READER, TActionType::CLOSE});
        LOGT << "m_map_uri_pipe.erase reader " << pipe->reader->uri;
        m_map_uri_pipe.erase(pipe->reader->uri);
    }

    if (pipe && pipe->writer && pipe->writer->uri && pipe->writer->uri != writer_uri) {
        m_actions.push_back(TAction{pipe->writer->uri, TStreamType::WRITER, TActionType::UNSUBSCRIBE});
        if (pipe->writer->uri != reader_uri)
            m_actions.push_back(TAction{pipe->writer->uri, TStreamType::WRITER, TActionType::CLOSE});
        LOGT << "m_map_uri_pipe.erase writer " << pipe->writer->uri;
        m_map_uri_pipe.erase(pipe->writer->uri);
    }

    if (!pipe && (reader_uri || writer_uri)) {
        pipe = TPipe::create();
        pipe->reader = TStream::create();
        pipe->writer = TStream::create();
        pipe->name = name;

        if (reader_uri)
            m_actions.push_back(TAction{reader_uri, TStreamType::READER, TActionType::OPEN});
        if (writer_uri)
            m_actions.push_back(TAction{writer_uri, TStreamType::WRITER, TActionType::OPEN});
    }

    // === ЛОГ: перед обновлением reader ===
    if (pipe) {
        LOGT << "setPipe before reader update: reader->uri=" << pipe->reader->uri
             << ", reader_uri=" << reader_uri
             << ", condition=" << (pipe->reader->uri != reader_uri);
    }

    if (pipe && pipe->reader->uri != reader_uri) {
        pipe->reader->uri = reader_uri;
        if (reader_uri) {
            m_actions.push_back(TAction{pipe->reader->uri, TStreamType::READER, TActionType::SUBSCRIBE});
            m_map_uri_pipe[pipe->reader->uri] = pipe;
        }
    }

    // === ЛОГ: перед обновлением writer ===
    if (pipe) {
        LOGT << "setPipe before writer update: writer->uri=" << pipe->writer->uri
             << ", writer_uri=" << writer_uri
             << ", condition=" << (pipe->writer->uri != writer_uri);
    }

    if (pipe && pipe->writer->uri != writer_uri) {
        pipe->writer->uri = writer_uri;
        if (writer_uri) {
            m_actions.push_back(TAction{pipe->writer->uri, TStreamType::WRITER, TActionType::SUBSCRIBE});
            m_map_uri_pipe[pipe->writer->uri] = pipe;
        }
    }

    if (pipe)
        m_map_name_pipe[name] = pipe;
    else
        m_map_name_pipe.erase(name);

    LOGT << "setPipe actions 2: " << m_actions;

    LOGT << "map_uri_pipe 11, " << reinterpret_cast<uint64_t>(this);
    for (auto const &uri_pipe : m_map_uri_pipe) {
        LOGT << "map_uri_pipe: " << uri_pipe.first
             << " " << uri_pipe.second->name
             << " " << uri_pipe.second->reader->uri
             << " " << uri_pipe.second->writer->uri;
    }
    LOGT << "map_uri_pipe 22";
}


void CContext::delPipe(
    std::string
        const &name)
{
    LOGT << "delPipe name: " << name;
    if (name.empty()) {
        for (auto const &name_pipe: m_map_name_pipe)
            setPipe(name_pipe.first, nullptr, nullptr);
    } else
        setPipe(name, nullptr, nullptr);
}


void CContext::delPipe(
    URI::TSharedPtr
        const &uri)
{
    LOGT << "delPipe uri: " << uri;
    if(uri) {
        if (auto pipe = getPipe(uri))
            delPipe(pipe->name);
    } else {
        delPipe("");
    }
}


void CContext::swapPipe(
    std::string
        const &name)
{
    LOGT << "swapPipe name: " << name;
    if (auto pipe = getPipe(name)) {
        setPipe(name, pipe->reader->uri, pipe->writer->uri);
        std::swap(pipe->writer->buffers, pipe->reader->buffers);
    } else
        throw std::runtime_error(
            "context swap pipe error: pipe not found by name '" + convert<std::string>(name) + "'");
}


void CContext::swapPipe(
    URI::TSharedPtr
        const &uri)
{
    LOGT << "swapPipe uri: " << uri;
    if (auto pipe = getPipe(uri)) {
        setPipe(pipe->name, pipe->writer->uri, pipe->reader->uri);
        std::swap(pipe->writer->buffers, pipe->reader->buffers);
    } else
        throw std::runtime_error(
            "swap pipe error: pipe not found by uri " + convert<std::string>(uri));
}


std::list<Buffer::TSharedPtr> CContext::getBuffers(
    std::string
        const &name,
    TStreamType
        const &stream_type)
{
    return getStream(name, stream_type)->buffers;
}


std::list<Buffer::TSharedPtr> CContext::getBuffers(
    URI::TSharedPtr
        const &uri,
    TStreamType
        const &stream_type)
{
    return getStream(uri, stream_type)->buffers;
}


void CContext::addBuffer(
    std::string
        const &name,
    TStreamType
        const &stream_type,
    Buffer::TSharedPtr
        const &buffer)
{
    assertExists(
        getStream(name, stream_type),
        "context add buffer error: stream " +
            convert<string>(stream_type) +
        " not found by name '" +
            convert<string>(name) + "'"
    )->buffers.push_back(buffer);
}


void CContext::addBuffer(
    URI::TSharedPtr
        const &uri,
    TStreamType
        const &stream_type,
    Buffer::TSharedPtr
        const &buffer)
{
    assertExists(
        getStream(uri, stream_type),
        "context add buffer error: stream " +
            convert<string>(stream_type) +
        " not found by uri " +
            convert<string>(uri)
    )->buffers.push_back(buffer);
}


void CContext::setPosition(
    URI::TSharedPtr
        const &uri,
    TStreamType
        const &stream_type,
    size_t
        const &position)
{
    auto stream = assertExists(
        getStream(uri, stream_type),
        "context set position error: stream " +
            convert<string>(stream_type) +
        " not found by uri " +
            convert<string>(uri)
    );

    if (stream->buffers.empty()) {
        stream->position = 0;
    } else {
        if (stream->buffers.front()->size() < position)
            stream->position = position;
        else {
            stream->position = 0;
            stream->buffers.pop_front();
        }
    }
}


size_t CContext::getPosition(
    URI::TSharedPtr
        const &uri,
    TStreamType
        const &stream_type)
{
    auto   stream = getStream(uri, stream_type);
    return stream->buffers.empty() ? 0 : stream->position;
}


std::list<CContext::TAction> CContext::getActions() {
    LOGT << "get actions: " << m_actions;

    struct TPipeEnd {
        TStreamType
            stream_type;
        URI::TSharedPtr
            uri;

        bool operator==(TPipeEnd const &other) const {
            return stream_type == other.stream_type && uri == other.uri;
        }
    };

    struct TPipeEndHash {
        size_t operator()(TPipeEnd const &pipe_end) const {
            return
                std::hash<int>()
                    (static_cast<int>(pipe_end.stream_type)) ^
               (std::hash<void*>()
                    (pipe_end.uri.get()) << 1);
        }
    };

    struct TActionNets {
        int open_close = 0;
        int subscribe_unsubscribe = 0;
    };

    std::unordered_map<TPipeEnd, TActionNets, TPipeEndHash>
        map_pipe_end_count;

    for (auto const &action: m_actions) {
        TPipeEnd pe { action.stream_type, action.uri };
        auto &count = map_pipe_end_count[pe];

        if (action.action_type == TActionType::OPEN)
            count.open_close++;
        else
        if (action.action_type == TActionType::CLOSE)
            count.open_close--;
        else
        if (action.action_type == TActionType::SUBSCRIBE)
            count.subscribe_unsubscribe++;
        else
        if (action.action_type == TActionType::UNSUBSCRIBE)
            count.subscribe_unsubscribe--;
    }

    std::list<TAction> optimized_actions;

    for (auto const &pipe_end_count: map_pipe_end_count) {
        if (pipe_end_count.second.open_close > 0)
            optimized_actions.push_back( { pipe_end_count.first.uri, pipe_end_count.first.stream_type, TActionType::OPEN } );
        else
        if (pipe_end_count.second.open_close < 0)
            optimized_actions.push_back( { pipe_end_count.first.uri, pipe_end_count.first.stream_type, TActionType::CLOSE } );

        if (pipe_end_count.second.subscribe_unsubscribe > 0)
            optimized_actions.push_back( { pipe_end_count.first.uri, pipe_end_count.first.stream_type, TActionType::SUBSCRIBE } );
        else
        if (pipe_end_count.second.subscribe_unsubscribe < 0)
            optimized_actions.push_back( { pipe_end_count.first.uri, pipe_end_count.first.stream_type, TActionType::UNSUBSCRIBE } );
    }

    optimized_actions.sort(
        [] (TAction const &a, TAction const &b) {
            return static_cast<int>(a.action_type) < static_cast<int>(b.action_type);
        }
    );

    m_actions.clear();

    LOGT << "get optimized_actions: " << optimized_actions;

    return optimized_actions;
}


CContext::TStream::TSharedPtr CContext::getStream(
    URI::TSharedPtr
        const &uri,
    TStreamType
        const &stream_type)
{
    auto uri_pipe  = m_map_uri_pipe.find(uri);
    if  (uri_pipe != m_map_uri_pipe.end()) {
        if (stream_type == TStreamType::READER)
            return uri_pipe->second->reader; // ----->

        if (stream_type == TStreamType::WRITER)
            return uri_pipe->second->writer; // ----->
    }

    LOGT << "map_uri_pipe 1, " << reinterpret_cast<uint64_t>(this);
    for (auto const &uri_pipe: m_map_uri_pipe) {
        LOGT
        << "map_uri_pipe: " << uri_pipe.first
        << " " << uri_pipe.second->name
        << " " << uri_pipe.second->reader->uri
        << " " << uri_pipe.second->writer->uri;
    }
    LOGT << "map_uri_pipe 2";

    throw std::runtime_error(
        "context getting stream error: " +
            convert<std::string>(stream_type) +
        " not found by uri " +
            convert<std::string>(uri)); // ----->
}


CContext::TStream::TSharedPtr CContext::getStream(
    std::string
        const &name,
    TStreamType
        const &stream_type)
{
    auto name_pipe  = m_map_name_pipe.find(name);
    if  (name_pipe != m_map_name_pipe.end()) {
        if (stream_type == TStreamType::READER)
            return name_pipe->second->reader; // ----->

        if (stream_type == TStreamType::WRITER)
            return name_pipe->second->writer; // ----->
    }

    throw std::runtime_error(
        "context getting stream error: " +
            convert<std::string>(stream_type) +
        " not found by name '" +
            name + "'"); // ----->
}


CContext::TPipe::TSharedPtr CContext::getPipe(
    std::string
        const &name)
{
    auto   name_pipe  = m_map_name_pipe.find(name);
    return name_pipe == m_map_name_pipe.end() ? nullptr : name_pipe->second;
}


CContext::TPipe::TSharedPtr CContext::getPipe(
    URI::TSharedPtr
        const &uri)
{
    auto   uri_pipe  = m_map_uri_pipe.find(uri);
    return uri_pipe == m_map_uri_pipe.end() ? nullptr : uri_pipe->second;
}


} // iridium::io::implementation
