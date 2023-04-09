// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_EVENT_8ED7D971_3E0C_4992_9B2A_B92BA12E998C
#define HEADER_EVENT_8ED7D971_3E0C_4992_9B2A_B92BA12E998C


#include "iridium/io/event.h"


namespace iridium {
namespace io {
namespace implementation {


class CEvent: public IEvent {
public:
    DEFINE_IMPLEMENTATION(CEvent)
    CEvent(IStream::TSharedPtr const &stream, TType const &type);

    void setType(TType const &type) override;
    TType               getType()   const override;
    IStream::TSharedPtr getStream() const override;

private:
    IStream::TSharedPtr m_stream;
    TType               m_type;
};



} // implementation
} // io
} // iridium


#endif // HEADER_EVENT_8ED7D971_3E0C_4992_9B2A_B92BA12E998C
