#ifndef HEADER_CONTEXT_89DD55FF_8778_4F30_8072_24E183C9568F
#define HEADER_CONTEXT_89DD55FF_8778_4F30_8072_24E183C9568F


// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "iridium/io/context.h"
#include "iridium/items.h"

#include <unordered_map>


namespace iridium::io::implementation {


class CContext: public IContextActions {
public:
    DEFINE_IMPLEMENTATION(CContext)
    CContext() = default;

    void setPipe(
        std::string
            const &name,
        URI::TSharedPtr
                   reader_uri,
        URI::TSharedPtr
                   writer_uri) override;
    void delPipe(
        std::string
            const &name) override;
    void delPipe(
        URI::TSharedPtr
            const &uri) override;

    void swapPipe(
        std::string
            const &name) override;
    void swapPipe(
        URI::TSharedPtr
            const &uri) override;

    std::list<Buffer::TSharedPtr>
        getBuffers(
            std::string
                const &name,
            TStreamType
                const &stream_type) override;
    std::list<Buffer::TSharedPtr>
        getBuffers(
            URI::TSharedPtr
                const &uri,
            TStreamType
                const &stream_type) override;
    void
        addBuffer(
            std::string
                const &name,
            TStreamType
                const &stream_type,
            Buffer::TSharedPtr
                const &buffer) override;
    void
        addBuffer(
            URI::TSharedPtr
                const &uri,
            TStreamType
                const &stream_type,
            Buffer::TSharedPtr
                const &buffer) override;

    void
        setPosition(
            URI::TSharedPtr
                const &uri,
            TStreamType
                const &stream_type,
            size_t
                const &pos) override;

    size_t
        getPosition(
            URI::TSharedPtr
                const &uri,
            TStreamType
                const &stream_type) override;

    std::list<TAction>
        getActions() override;

private:
    struct TStream {
        DEFINE_CREATE(TStream)
        URI::TSharedPtr
            uri;
        std::list<Buffer::TSharedPtr>
            buffers;
        size_t
            position;
    };
    struct TPipe {
        DEFINE_CREATE(TPipe)
        TStream::TSharedPtr
            reader;
        TStream::TSharedPtr
            writer;
        std::string
            name;
    };

    TStream::TSharedPtr
        getStream(
            URI::TSharedPtr
                const &uri,
            TStreamType
                const &stream_type);
    TStream::TSharedPtr
        getStream(
            std::string
                const &name,
            TStreamType
                const &stream_type);
    TPipe::TSharedPtr
        getPipe(
            std::string
                const &name);
    TPipe::TSharedPtr
        getPipe(
            URI::TSharedPtr
                const &uri);

    std::list<TAction>
        m_actions;
    std::unordered_map<std::string, TPipe::TSharedPtr>
        m_map_name_pipe;
    std::unordered_map<URI::TSharedPtr, TPipe::TSharedPtr>
        m_map_uri_pipe;
};


} // iridium::io::implementation


#endif // HEADER_CONTEXT_89DD55FF_8778_4F30_8072_24E183C9568F
