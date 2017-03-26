#include "json_parser.h"

#include <map>
#include <list>
#include <json/json.h>

#include "utility/pasring/implementation/node.h"
#include "utility/assert.h"


using std::string;
using std::map;
using std::list;
using Json::Value;
using Json::Reader;
using Json::FastWriter;


namespace utility {
namespace parsing {
namespace implementation {


void convertJsonNodeToNode(Value const &json_node, INode::TSharedPtr node) {
    for (auto json_node_item = json_node.begin(); json_node_item != json_node.end(); json_node_item++) {
        string json_node_name = json_node_item.name();

        switch (json_node_item->type()) {
        case Json::arrayValue:
            for (auto json_array_item = json_node_item->begin(); json_array_item != json_node_item->end(); json_array_item++) {
                auto node_child = CNode::create(json_node_name);
                convertJsonNodeToNode(*json_array_item, node_child); // <-----
                node->addChild(node_child);
            }
            break; // --->
        case Json::objectValue:
            if (json_node_name.empty()) {
                for (auto json_object_item = json_node_item->begin(); json_object_item != json_node_item->end(); json_object_item++)
                    convertJsonNodeToNode(*json_object_item, node); // <-----
            } else {
                auto object_node = CNode::create(json_node_name);
                convertJsonNodeToNode(*json_node_item, object_node); // <-----
                node->addChild(object_node);
            }
            break; // --->
        default:
            string name     = json_node_name;
            string value    = json_node_item->asString();

            if (name == "#text")
                name = "";

            node->addChild(name, value);
        }
    }
}


void convertNodeToJsonNode(INode::TConstSharedPtr const &node, Value &json_node_) {
    map<string, list<INode::TConstSharedPtr> > m;

    for (auto const &i: *node)
        m[i->getName()].push_back(i);

    for (auto const &i: m) {
        Value json_child;

        if (i.second.size() == 1) {
            auto node_child = *i.second.begin();

            if (node_child->hasChilds()) {
                convertNodeToJsonNode(node_child, json_child); // <-----
                json_node_[i.first] = json_child;
            } else {
                string name = node_child->getName();

                if (name.empty())
                    name = "#text";

                json_node_[name] = node_child->getValue();
            }
        } else {
            for (auto const &ii: i.second) {
                Value json_item;
                convertNodeToJsonNode(ii, json_item); // <-----
                json_child.append(json_item);
            }
            json_node_[i.first] = json_child;
        }
    }
}


void CJSONParser::parse(std::string const &source) {
    Value           root;
    Json::Reader    reader;
    bool            is_parsed = reader.parse(source, root);

    if (is_parsed) {
        m_root_node = CNode::create("root", "");
        convertJsonNodeToNode(root, m_root_node);
        if (m_root_node->size() == 1)
            m_root_node = *m_root_node->begin();
        else
            throw std::runtime_error("json parsing error: empty nodes"); // ----->
    } else
        throw std::runtime_error("json parsing error: recursive parse error"); // ----->
}


std::string CJSONParser::compose() const {
    Value root(Json::objectValue);
    convertNodeToJsonNode(m_root_node, root);
    //Json::FastWriter writer;
    Json::StyledWriter writer;
    return writer.write(root);
}


} // implementation
} // parsing
} // utility
