#include "event.h"
#include <unordered_set>


namespace iridium::io {


Event::Event(IStream::TSharedPtr const &stream_, TOperation const &operation_, TStatus const &status_)
:
    stream      (stream_),
    operation   (operation_),
    status      (status_)
{}


} // namespace iridium::io


size_t std::hash<iridium::io::Event>::operator()
    (iridium::io::Event const &e) const
{
    size_t hash = 0;

    if (e.stream) {
        for (auto const &handle: e.stream->getHandles())
            hash ^= static_cast<size_t>(handle) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }

    hash ^= static_cast<size_t>(e.operation) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= static_cast<size_t>(e.status)    + 0x9e3779b9 + (hash << 6) + (hash >> 2);

    return hash; // ----->
}


size_t std::hash<iridium::io::Event::TSharedPtr>::operator()
    (iridium::io::Event::TSharedPtr const &e) const
{
    return e ? std::hash<iridium::io::Event>()(*e) : 0; // ----->
}


namespace {


std::string convertEventToString(iridium::io::Event const &event) {
    using iridium::convertion::convert;
    return
        "\n{\n  "   + convert<std::string>(event.stream)
        + ",\n  "   + convert<std::string>(event.operation)
        + ",\n  "   + convert<std::string>(event.status)
        +  "\n}\n";
}


} // unnamed


IMPLEMENT_ENUM(iridium::io::Event::TOperation)
IMPLEMENT_ENUM(iridium::io::Event::TStatus)
IMPLEMENT_CONVERT(std::string, iridium::io::Event, convertEventToString)
