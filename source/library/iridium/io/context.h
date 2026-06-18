// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONTEXT_EDB02699_B5EA_40D9_B41B_28263B6EEFE0
#define HEADER_CONTEXT_EDB02699_B5EA_40D9_B41B_28263B6EEFE0


#include "pipe.h"


namespace iridium::io {


class IContext {
public:
    DEFINE_INTERFACE(IContext);

    DEFINE_ENUM(
        TStreamType,
        READER,
        WRITER
    );

    virtual void setPipe(
        std::string
            const &name,
        URI::TSharedPtr
            const &reader,
        URI::TSharedPtr
            const &writer) = 0;
    virtual void delPipe(
        std::string
            const &name) = 0;
    virtual void delPipe(
        URI::TSharedPtr
            const &uri) = 0;

    virtual void swapPipe(
        std::string
            const &name) = 0;
    virtual void swapPipe(
        URI::TSharedPtr
            const &uri) = 0;

    virtual std::list<Buffer::TSharedPtr>
        getBuffers(
            std::string
                const &name,
            TStreamType
                const &stream_type) = 0;
    virtual std::list<Buffer::TSharedPtr>
        getBuffers(
            URI::TSharedPtr
                const &uri,
            TStreamType
                const &stream_type) = 0;
    virtual void
        addBuffer(
            std::string
                const &name,
            TStreamType
                const &stream_type,
            Buffer::TSharedPtr
                const &buffer) = 0;
    virtual void
        addBuffer(
            URI::TSharedPtr
                const &uri,
            TStreamType
                const &stream_type,
            Buffer::TSharedPtr
                const &buffer) = 0;
};


class IContextActions: public IContext {
public:
    DEFINE_INTERFACE(IContextActions);
    DEFINE_ENUM(
        TActionType,
        OPEN,
        CLOSE,
        SUBSCRIBE,
        UNSUBSCRIBE
    );
    struct TAction {
        TStreamType
            stream_type;
        TActionType
            action_type;
        URI::TSharedPtr
            uri;
    };

    virtual std::list<TAction>
        getActions() = 0;

    virtual void
        setPosition(
            URI::TSharedPtr
                const &uri,
            TStreamType
                const &stream_type,
            size_t
                const &pos) = 0;

    virtual size_t
        getPosition(
            URI::TSharedPtr
                const &uri,
            TStreamType
                const &stream_type) = 0;
};


} // iridium::io


DEFINE_CONVERT(std::string, iridium::io::IContextActions::TAction)
DEFINE_ENUM_CONVERT(iridium::io::IContext::TStreamType)
DEFINE_ENUM_CONVERT(iridium::io::IContextActions::TActionType)


#endif // HEADER_CONTEXT_EDB02699_B5EA_40D9_B41B_28263B6EEFE0
