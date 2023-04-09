#include "parser_http.h"

#include "iridium/items.h"
#include "iridium/strings.h"
#include "iridium/assert.h"
#include "iridium/parsing/implementation/node.h"

#include <unordered_map>
#include <unordered_set>
#include <list>
#include <string>


//#include <iostream>
//using namespace std;


using std::unordered_map;
using std::unordered_set;
using std::list;
using std::string;
using iridium::convertion::convert;


/*
HTTP - Requests
 - A Request-line
 - Zero or more header (General|Request|Entity) fields followed by CRLF
 - An empty line (i.e., a line with nothing preceding the CRLF) indicating the end of the header fields
 - Optionally a message-body

HTTP - Responses
 - A Status-line
 - Zero or more header (General|Response|Entity) fields followed by CRLF
 - An empty line (i.e., a line with nothing preceding the CRLF) indicating the end of the header fields
 - Optionally a message-body

node:
 - message
 - headers list
 - body
*/


namespace {


static string const HTTP           = "http";
static string const HTTP_MESSAGE   = "message";
static string const HTTP_HEADERS   = "headers";
static string const HTTP_BODY      = "body";


}


namespace iridium {
namespace parsing {
namespace implementation {


INode::TSharedPtr CHTTPParser::parse(string const &source) const {
    auto node = CNode::create(HTTP);

    unordered_map<string, unordered_set<string> > cache;
    list<string>    names;
    string          body;
    auto            lines = split(source, "\n\n");

    if (lines.size() > 1) {
        auto tmp = lines.front();
        lines.pop_front();
        for (auto const &line: lines)
            body += line;
        lines = split(tmp, "\n");
    } else
        lines = split(source, "\n");

    node->addChild(HTTP_MESSAGE, lines.front());
    lines.pop_front();

    for (auto const &line_: lines) {
        auto line   = trim(line_);
        if (line.empty())
            continue; // <---
        auto arg    = assertSize(split(line, ":", 2), 2, "http header parsing error: line '" + line + "'");
        auto name   = lowerCase(arg.front());

        addUnique(name, names);

        arg.pop_front();

        list<string> values;
        if (name == "last-modified" || name == "date")
            values.push_back(arg.front());
        else
            values = split(arg.front(), ",");

        if (values.size() == 1)
            cache[name].clear();

        for (auto const &value: values)
            cache[name].insert(trim(value));
    }

    auto headers = node->addChild(HTTP_HEADERS);
    for (auto const &name: names)
        for (auto const &value: cache[name])
            headers->addChild(name, value);

    node->addChild(HTTP_BODY, body);
    return node; // ----->
}


string CHTTPParser::compose(INode::TConstSharedPtr const &node) const {
    string result;

    auto message = assertExists(node->getChild(HTTP_MESSAGE), "http composing error: node " + HTTP_MESSAGE + " does not exists");
    result      += message->getValue() + "\n";

    if (node) {
        unordered_map<string, unordered_set<string> > cache;
        list<string> names;

        auto headers = assertExists(node->getChild(HTTP_HEADERS), "http composing error: node " + HTTP_HEADERS + " does not exists");
        for (auto const &header: *headers) {
            addUnique(header->getName(), names);
            cache[header->getName()].insert(header->getValue());
        }

        for (auto const &name: names) {
            result += name + ": ";

            for (auto const &value: cache[name])
                result += value + ", ";

            result.replace(result.size() - 2, 2, "\n");
        }

    } else
        throw std::runtime_error("http composing error: http header root node is null"); // ----->

    auto body   = assertExists(node->getChild(HTTP_BODY), "http composing error: node " + HTTP_BODY + " does not exists");
    result     += "\n" + body->getValue();

    return result; // ----->
}


} // implementation
} // parser
} // iridium
