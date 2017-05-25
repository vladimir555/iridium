#include "parser_json.h"

#include <map>
#include <list>

#include "utility/parsing/implementation/node.h"
#include "utility/convertion/convert.h"
#include "utility/convertion/type_name.h"
#include "utility/assert.h"
#include "utility/strings.h"


using std::string;
using std::map;
using std::list;


namespace {


static const string DEFAULT_TAB = "    ";


}


namespace utility {
namespace parsing {
namespace implementation {


void convertStringToNode(string const &source, INode::TSharedPtr node, size_t &index, bool const &is_array_item = false) {
    enum class TMarker {
        NAME,
        VALUE
    };

    string  name;
    string  value;
    TMarker marker   = TMarker::NAME;

    auto clearFields = [&name, &value] () {
        name.clear();         
        value.clear(); 
    };
    auto trimFields  = [&name, &value] () {
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
        &trimFields, 
        &clearFields
    ] () { 
        trimFields(); 
        if (!name.empty() && !value.empty()) {
            if (name == "#text")
                name.clear();

            if (isdigit(static_cast<int>(name[0])))
                throw std::runtime_error("wrong json field name: " + name); // ----->

            for (auto const ch : name)
                if (ispunct(static_cast<int>(ch)))
                    throw std::runtime_error("wrong json field name: " + name); // ----->

            node->addChild(name, value);
        }
        clearFields();
    };

    while (index < source.size()) {
        auto ch = source[index];
        index++;

        if (ch == '{') {
            INode::TSharedPtr node_child;

            trimFields();

            if (name.empty())
                node_child = node;
            else
                node_child = node->addChild(name, "");

            convertStringToNode(source, node_child, index); // <-----

            if (is_array_item)
                return; // <-----

            clearFields();
            continue; // <---
        }

        if (ch == '[') {
            while (ch != ']') {
                trimFields();
                INode::TSharedPtr node_child = CNode::create(name, "");
                convertStringToNode(source, node_child, index, true); // <-----
                if (node_child->hasChilds())
                    node->addChild(node_child);
                ch = source[index - 1];
            }

            clearFields();
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
    else
        throw std::runtime_error("json parsing error"); // ----->

    return root_node; // ----->
}


void convertNodeToJsonString(INode::TConstSharedPtr const &node, string &result, string const &tab = "", INode::TConstSharedPtr const &node_type = nullptr) {
    struct TJSONEntry { 
        INode::TConstSharedPtr node; 
        INode::TConstSharedPtr node_type; 
    };

    map<string, list< TJSONEntry > > m;

    if (node_type) {
        auto inode      = node->begin();
        auto inode_type = node_type->begin();
        while (inode != node->end()) {
            if (inode_type != node_type->end()) {
                m[(*inode)->getName()].push_back({ *inode, *inode_type });
                inode_type++;
            } else
                m[(*inode)->getName()].push_back({ *inode, nullptr });
            inode++;
        }
    } else
        for (auto const &i : *node)
            m[i->getName()].push_back( { i, nullptr } );

    for (auto const &i : m) {
        string line_end;
        if (m.size() == 1 || i.first == (--m.end())->first)
            line_end = "\n";
        else
            line_end = ",\n";

        if (i.second.size() == 1) {
            auto node_child = *i.second.begin();

            if (node_child.node->hasChilds()) {
                result += tab + node_child.node->getName() + ": {\n";
                convertNodeToJsonString(node_child.node, result, tab + DEFAULT_TAB, node_child.node_type); // <-----
                result += tab + "}" + line_end;
            } else {
                string name = node_child.node->getName();

                if (name.empty())
                    name = "#text";

                string value_interpretation;
                if (node_child.node_type) {
                    if (node_child.node_type->getValue() == convertion::getTypeName<string>())
                        value_interpretation = "\"" + node_child.node->getValue() + "\"";
                    else
                        value_interpretation = node_child.node->getValue();
                } else
                    value_interpretation = "\"" + node_child.node->getValue() + "\"";
                    
                if (tab.empty())
                    result += tab + DEFAULT_TAB + name + ": " + value_interpretation + line_end;
                else
                    result += tab + name + ": " + value_interpretation + line_end;
            }
        } else {
            result += tab + i.first + ": [\n";
            for (auto const &ii : i.second) {
                string line_end_;
                if (i.second.size() == 1 || ii.node == (--i.second.end())->node)
                    line_end_ = "\n";
                else
                    line_end_ = ",\n";

                result += tab + DEFAULT_TAB + "{\n";
                convertNodeToJsonString(ii.node, result, tab + DEFAULT_TAB + DEFAULT_TAB, ii.node_type); // <-----
                result += tab + DEFAULT_TAB + "}" + line_end_;
            }
            result += tab + "]" + line_end;
        }
    }
}


string CJSONParser::compose(INode::TConstSharedPtr const &root_node, INode::TConstSharedPtr const &root_node_types) const {
    string result;
    convertNodeToJsonString(root_node, result, DEFAULT_TAB, root_node_types);
    result = "{\n" + result + "}\n";
    return result; // ----->
}


} // implementation
} // parser
} // utility
