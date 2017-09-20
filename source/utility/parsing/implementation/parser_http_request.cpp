#include "parser_http_request.h"

#include "utility/strings.h"
#include "utility/assert.h"
#include "utility/parsing/implementation/node.h"

#include <unordered_map>
#include <unordered_set>
#include <list>
#include <string>


using std::unordered_map;
using std::unordered_set;
using std::list;
using std::string;


namespace utility {
namespace parsing {
namespace implementation {


INode::TSharedPtr CHTTPRequestParser::parse(std::string const &source) const {
    INode::TSharedPtr node = CNode::create("http-request");

    unordered_map<string, unordered_set<string> > cache;
    list<string> names;

    for (auto const &line: split(source, "\n")) {
        auto arg    = assertCount(split(line, ":"), 2, "http header parsing error line '" + line + "'");
        auto name   = lowerCase(arg[0]);

        if (std::find(names.begin(), names.end(), name) == names.end())
            names.push_back(name);

        auto values = split(arg[1], ",");
        if (values.size() == 1)
            cache[name].clear();

        for (auto const &value: values)
            cache[name].insert(trim(lowerCase(value)));
    }

    for (auto const &name: names) {
        for (auto const &value: cache[name])
            node->addChild(name, value);
    }

    return node;
}


std::string CHTTPRequestParser::compose(INode::TConstSharedPtr const &root_node, INode::TConstSharedPtr const &root_node_types) const {

}


} // implementation
} // parser
} // utility
