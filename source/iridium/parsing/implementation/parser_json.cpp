/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "parser_json.h"

#include <unordered_map>
#include <list>

#include "iridium/parsing/implementation/node.h"
#include "iridium/convertion/convert.h"
#include "iridium/convertion/type_name.h"
#include "iridium/assert.h"
#include "iridium/strings.h"
#include "iridium/items.h"


using std::string;
using std::unordered_map;
using std::list;


namespace {


static const string DEFAULT_TAB = "    ";


}


#include "iridium/logging/logger.h"
namespace iridium {
namespace parsing {
namespace implementation {


void convertStringToNode(string const &source, INode::TSharedPtr node, size_t &index, bool const &is_array_item = false) {
    enum class TMarker {
        NAME,
        VALUE
    };

    string  name;
    string  value;
    TMarker marker      = TMarker::NAME;

    auto clearEntry     = [&name, &value] () {
        name.clear();         
        value.clear(); 
    };
    auto validateName   = [&name] () {
        if (name.empty() || name == "#text")
            return; // ----->

        LOGT << "name = '" << name << "'";
        if (name[0] >= '0' && name[0] <= '9')
            throw std::runtime_error("wrong json field name: " + name); // ----->

        for (auto const ch: name)
            if (ch != '.' && ispunct(static_cast<int>(ch)))
                throw std::runtime_error("wrong json field name: " + name); // ----->
    };
    auto trimEntry      = [&name, &value] () {
        name    = trim(name , " \t\n\r\"");
        value   = trim(value, " \t\n\r\"");
        if (name.substr(0, 2)  == "\\\"")
            name  += "\"";
        if (value.substr(0, 2) == "\\\"")
            value += "\"";
    };
    auto addEntry    = [
        &name, 
        &value, 
        &node,
        &validateName,
        &trimEntry,
        &clearEntry,
        &is_array_item
    ] () { 
        trimEntry();
        if (!name.empty() && !value.empty()) {
            if (name == "#text")
                name.clear();

            validateName();
            node->addChild(name, value);
        } else {
            // array simple item
            if (is_array_item && !name.empty() && value.empty())
                node->addChild("", name);
            else {
                // todo: error
            }
        }
        clearEntry();
    };

    while (index < source.size()) {
        auto ch  = source[index];
        index++;

        if (ch == '{') {
            INode::TSharedPtr node_child;

            trimEntry();
            validateName();

            if (name.empty())
                node_child = node;
            else
                node_child = node->addChild(name, "");

            convertStringToNode(source, node_child, index); // <-----

            if (is_array_item)
                return; // <-----

            clearEntry();
            continue; // <---
        }

        if (ch == '[') {
            while (ch != ']') {
                trimEntry();
                INode::TSharedPtr node_array_item_tmp = CNode::create(name);
                convertStringToNode(source, node_array_item_tmp, index, true); // <-----
                if (node_array_item_tmp->hasChilds()) {
                    auto node_array_item = CNode::create(name);
                    for (auto const &child: *node_array_item_tmp)
                        if (child->getName().empty())
                            // simple array item like [1, 2, ... ]
                            node->addChild(name, child->getValue());
                        else
                            // object array item like [ { ... },  { ... }, ...]
                            node_array_item->addChild(child);
                    if (node_array_item->hasChilds())
                        node->addChild(node_array_item);
                }
                ch = source[index - 1];
            }

            clearEntry();
            continue; // <---
        }

        if (ch == ',') {
            addEntry();
            marker = TMarker::NAME;
            continue; // <---
        }

        if (ch == '}' || ch == ']') {
            addEntry();
            return; // ----->
        }

        if (ch == ':') {
            marker = TMarker::VALUE;
            continue; // <---
        }

        if (marker == TMarker::NAME)
            name  += ch;

        if (marker == TMarker::VALUE)
            value += ch;
    }
}


INode::TSharedPtr CJSONParser::parse(std::string const &source) const {
    INode::TSharedPtr   root_node = CNode::create("root", "");
    size_t              index = 0;

    convertStringToNode(source, root_node, index);

    if (root_node->size() == 1)
        root_node = *root_node->begin();
    else {
        auto childs = root_node;
        root_node = CNode::create("root");
        root_node->addChild(childs);
    }

    return root_node; // ----->
}


void convertNodeToJsonString(INode::TConstSharedPtr const &node, string &result, string const &tab = "") {
    unordered_map<string, list< INode::TConstSharedPtr > > map_name_node;
    list<string> names;

    for (auto const &i : *node) {
        addUnique(i->getName(), names);
        map_name_node[i->getName()].push_back(i);
    }

    size_t name_number = 0;
    for (auto const &name: names) {
        auto    nodes = map_name_node[name];
        string  line_end;

        name_number++;
        if (map_name_node.size() == 1 || name_number == names.size())
            line_end = "\n";
        else
            line_end = ",\n";

        if (nodes.size() == 1) {
            auto node_child = *nodes.begin();

            if (node_child->hasChilds()) {
                result += tab + "\"" + node_child->getName() + "\"" + ": {\n";
                convertNodeToJsonString(node_child, result, tab + DEFAULT_TAB); // <-----
                result += tab + "}" + line_end;
            } else {
                string name = node_child->getName();

                if (name.empty())
                    name = "#text";
                    
                result += tab;
                if (tab.empty())
                    result += DEFAULT_TAB;
                result += "\"" + name + "\"" + ": " + "\"" + node_child->getValue() + "\"" + line_end;
            }
        } else {
            result += tab + "\"" + name + "\"" + ": [\n";
            size_t node_number = 0;
            for (auto const &node : nodes) {
                string line_end_;
                node_number++;
                if (nodes.size() == 1 || node_number == nodes.size())
                    line_end_ = "\n";
                else
                    line_end_ = ",\n";

                if (node->hasChilds()) {
                    result += tab + DEFAULT_TAB + "{\n";
                    convertNodeToJsonString(node, result, tab + DEFAULT_TAB + DEFAULT_TAB); // <-----
                    result += tab + DEFAULT_TAB + "}" + line_end_;
                } else {
                    result += tab + DEFAULT_TAB + "\"" + node->getValue() + "\"" + line_end_;
                }
            }
            result += tab + "]" + line_end;
        }
    }
}


string CJSONParser::compose(INode::TConstSharedPtr const &root_node) const {
    string result;
    convertNodeToJsonString(root_node, result, DEFAULT_TAB + DEFAULT_TAB);
    result = "{\n" + DEFAULT_TAB + "\"" + root_node->getName() + "\"" + ": {\n" + result + DEFAULT_TAB + "}\n}\n";
    return result; // ----->
}


} // implementation
} // parser
} // iridium
