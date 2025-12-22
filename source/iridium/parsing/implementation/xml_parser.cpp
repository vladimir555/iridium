#include "xml_parser.h"

#include <sstream>
#include <vector>
#include <list>
#include <unordered_map>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

#include "node.h"
#include "iridium/assert.h"
#include "iridium/items.h"
#include "iridium/strings.h"


using TXMLDocument  = rapidxml::xml_document<>;
using TXMLNode      = rapidxml::xml_node<>;
using TXMLAttribute = rapidxml::xml_attribute<>;


using std::stringstream;
using std::istreambuf_iterator;
using std::string;
using std::vector;
using std::list;
using std::unordered_map;


namespace {


string mask(string const &source) {
//    string result;
//    for (auto const &ch: source) {
//        if (iridium::checkOneOf(ch, /*'&', '\'', '<', '>', */'\n'))
//            result += '\\';
//        result += ch;
//    }
//    return result;
    return source;
}


}


namespace iridium::parsing::implementation {


INode::TSharedPtr convertXMLNodeToNode(TXMLNode const * const xml_node) {
    INode::TSharedPtr node;
    if (xml_node && xml_node->name_size() > 0) {
        node = CNode::create(xml_node->name());

        auto xml_node_attribute = xml_node->first_attribute();
        while (xml_node_attribute) {
            node->addChild(xml_node_attribute->name(), xml_node_attribute->value());
            xml_node_attribute = xml_node_attribute->next_attribute();
        }

        if(xml_node->value_size() > 0) {
            if (xml_node->first_attribute() == nullptr &&
               (xml_node->first_node()      == nullptr ||
                xml_node->first_node()->name_size() == 0))
            {
                return CNode::create(xml_node->name(), xml_node->value()); // ----->
            } else
                node->addChild("", xml_node->value());
        }

        auto xml_node_child = xml_node->first_node();
        while (xml_node_child) {
            INode::TSharedPtr node_child = convertXMLNodeToNode(xml_node_child); // <-----
            if (node_child)
                node->addChild(node_child);

            xml_node_child = xml_node_child->next_sibling();
        }
    }
    return node; // ----->
}


TXMLNode *convertNodeToXMLNode(INode::TConstSharedPtr const &node, TXMLNode *xml_node_, TXMLDocument &xml_document) {
    if (node->hasChilds()) {
        auto name           = xml_document.allocate_string(mask(node->getName()).c_str());
        auto xml_node_child = xml_document.allocate_node(rapidxml::node_element, name);

        unordered_map<string, int> m;
        for (auto const &child: *node)
            m[child->getName()]++;

        for (auto const &child: *node) {
            if (!child->hasChilds() && m[child->getName()] > 1) {
                auto name_ = xml_document.allocate_string(mask(child->getName()).c_str());
                auto value = xml_document.allocate_string(mask(child->getValue()).c_str());
                auto xml_node_array_item = xml_document.allocate_node(rapidxml::node_element, name_);
                xml_node_array_item->value(value);
                xml_node_child->append_node(xml_node_array_item);
            } else
                convertNodeToXMLNode(child, xml_node_child, xml_document); // <-----
        }

        if (xml_node_)
            xml_node_->append_node(xml_node_child);
        else
            xml_node_ = xml_node_child;
    } else {
        assertExists(xml_node_, "xml_node does not exists");
        auto value = xml_document.allocate_string(mask(node->getValue()).c_str());

        if (node->getName().empty()) {
            xml_node_->value(value);
        } else {
            auto name           = xml_document.allocate_string(mask(node->getName()).c_str());
            auto xml_attribute  = xml_document.allocate_attribute(name, value);
            xml_node_->append_attribute(xml_attribute);
        }
    }
    return xml_node_; // ----->
}


INode::TSharedPtr CXMLParser::parse(string const &source) const {
    stringstream ss(source);
    TXMLDocument xml_document;
    vector<char> buffer((istreambuf_iterator<char>(ss)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    xml_document.parse<0>(&buffer[0]);
    auto xml_root_node  = assertExists(xml_document.first_node(), "root node does not exists"); // ----->
    return convertXMLNodeToNode(xml_root_node); // ----->
}


string CXMLParser::compose(INode::TConstSharedPtr const &root_node) const {
    assertExists(root_node, "xml compose error: node is null");

    TXMLDocument xml_document;

    auto xml_node_declaration = xml_document.allocate_node(rapidxml::node_declaration);
    xml_node_declaration->append_attribute(xml_document.allocate_attribute("version"    , "1.0"));
    xml_node_declaration->append_attribute(xml_document.allocate_attribute("encoding"   , "utf-8"));
    xml_document.append_node(xml_node_declaration);

    auto xml_root_node = convertNodeToXMLNode(root_node, nullptr, xml_document);
    xml_document.append_node(xml_root_node);

    string xml_as_string;
    rapidxml::print(std::back_inserter(xml_as_string), xml_document, rapidxml::print_ident_spaces | rapidxml::print_no_expand_quote_in_text);

    return xml_as_string; // ----->
}


} // iridium::parsing::implementation
