/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "listener.h"


IMPLEMENT_ENUM(iridium::io::Event::TType)


namespace iridium {
namespace io {


Event::Event(
    TType                   const &type_,
    IStream::TSharedPtr     const &stream_)
:
    type    (type_),
    stream  (stream_)
{}


} // io
} // iridium
