#include "parser_command_line.h"


#include "iridium/strings.h"
#include "node.h"

#include <string_view>


namespace iridium {
namespace parsing {
namespace implementation {


INode::TSharedPtr CCommandLineParser::parse(std::string const &source) const {
    INode::TSharedPtr node = CNode::create("root");

    for (std::string_view item: split(source, " ")) {
        if (item.substr(0, 2) == "--") {
            item.remove_prefix(2);
            auto i = item.find('=');
            if (i == std::string::npos)
                throw std::runtime_error("command line parsing error: argument '" + std::string(item) + "', source '" + source + "'");
            node->addChild(std::string(item.substr(0, i)), std::string(item.substr(i + 1)));
            continue; // <---
        }
        if (item.substr(0, 1) == "-") {
            item.remove_prefix(1);
            if (item.size() == 1 && std::isalpha(item.back()))
                node->addChild(std::string(item), convertion::convert<std::string>(true));
            else
                throw std::runtime_error("command line parsing error: argument '" + std::string(item) + "', source '" + source + "'");
        }
    }

    return node; // ----->
}


std::string CCommandLineParser::compose(INode::TConstSharedPtr const &root_node) const {
    return "";
}


} // implementation
} // parsing
} // iridium
