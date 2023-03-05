#ifndef HEADER_PARSER_COMMAND_LINE_C61F805F_9FDA_42C8_95BB_139F1120D9E9
#define HEADER_PARSER_COMMAND_LINE_C61F805F_9FDA_42C8_95BB_139F1120D9E9


#include "node.h"


namespace iridium {
namespace parsing {
namespace serialization {
} // serialization
} // parsing
} // iridium



#define DEFINE_COMMAND_LINE_BEGIN(help)


#define DEFINE_COMMAND_LINE_ARG_BEGIN_4(type, name, help, default_value)


#define DEFINE_COMMAND_LINE_ARG_END()


#define DEFINE_COMMAND_LINE_END()


#define DEFINE_COMMAND_LINE_ARG_4(type, name, help, default_value)


#define DEFINE_COMMAND_LINE_ARG(...) \
    dMACRO_CHOOSER(DEFINE_ATTRIBUTE, __VA_ARGS__)(__VA_ARGS__)


#define DEFINE_COMMAND_LINE_ARG_BEGIN(...) \
    dMACRO_CHOOSER(DEFINE_ATTRIBUTE, __VA_ARGS__)(__VA_ARGS__)


#endif // HEADER_PARSER_COMMAND_LINE_C61F805F_9FDA_42C8_95BB_139F1120D9E9
