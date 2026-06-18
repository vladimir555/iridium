#include "event.h"


using iridium::convertion::convert;


std::string convert_(iridium::io::TEvent const &event) {
    return "{ operation: " + convert<std::string>(event.operation) + ", uri: " + convert<std::string>(event.uri) + " }";
}


// IMPLEMENT_ENUM(iridium::io::TEvent::TOperation);
IMPLEMENT_CONVERT(std::string, iridium::io::TEvent, convert_);
