#include "context.h"


using iridium::convertion::convert;
using iridium::io::IContextActions;
using iridium::io::IContext;
using std::string;


std::string convert_(IContextActions::TAction const &source) {
    return
        "{ stream_type: "
            + convert<string>(source.stream_type) +
        ", action_type: "
            + convert<string>(source.action_type) +
        ", uri: "
            + convert<string>(source.uri) + " }";
};


IMPLEMENT_CONVERT(std::string, IContextActions::TAction, convert_)
// IMPLEMENT_ENUM(IContext::TStreamType)
// IMPLEMENT_ENUM(IContextActions::TActionType)


namespace iridium::io::implementation {
} // iridium::io::implementation
