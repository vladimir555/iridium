// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347
#define HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347


#include "iridium/enum.h"

#include "stream.h"


namespace iridium::io {


struct Event {
    DEFINE_CREATE(Event)

    DEFINE_ENUM(
        TOperation,
        OPEN,
        READ,
        WRITE,
        TIMEOUT,
        ERROR_,
        CLOSE)

    DEFINE_ENUM(
        TStatus,
        BEGIN,
        END)

    Event(IStream::TSharedPtr const &stream, TOperation const &operation, TStatus const &status = TStatus::UNKNOWN);

    IStream::TSharedPtr stream;
    TOperation          operation;
    TStatus             status;
};


} // namespace iridium::io


template<>
struct std::hash<iridium::io::Event> {
    size_t operator()(iridium::io::Event const &e) const;
};


template<>
struct std::hash<iridium::io::Event::TSharedPtr> {
    size_t operator()(iridium::io::Event::TSharedPtr const &e) const;
};


DEFINE_ENUM_CONVERT(iridium::io::Event::TOperation)
DEFINE_ENUM_CONVERT(iridium::io::Event::TStatus)
DEFINE_CONVERT(std::string, iridium::io::Event)


#endif // HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347
