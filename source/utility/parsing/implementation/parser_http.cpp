#include "parser_http.h"

#include "utility/items.h"
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


INode::TSharedPtr CHTTPParser::parse(std::string const &source) const {
    INode::TSharedPtr node = CNode::create("http");

    unordered_map<string, unordered_set<string> > cache;
    list<string> names;

    for (auto const &line: split(source, "\n")) {
        auto arg    = assertCount(split(line, ":"), 2, "http header parsing error line '" + line + "'");
        auto name   = lowerCase(arg[0]);

        addUnique(name, names);

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

    return node; // ----->
}


std::string CHTTPParser::compose(INode::TConstSharedPtr const &root_node) const {
    string result;

    if (root_node) {
        unordered_map<string, unordered_set<string> > cache;
        list<string> names;

        for (auto const &node: *root_node) {
            if (node->hasChilds())
                throw std::runtime_error("http header nodes can't have childs"); // ----->
            else {
                string name     = node->getName();
                string value    = node->getValue();

                addUnique(name, names);
                cache[name].insert(value);
            }
        }

        for (auto const &name: names) {
            result += name + ": ";

            for (auto const &value: cache[name])
                result += value + ", ";

            result.replace(result.size() - 2, 2, "\n");
        }

    } else
        throw std::runtime_error("http header root node is null"); // ----->

    return result; // ----->
}


} // implementation
} // parser
} // utility
