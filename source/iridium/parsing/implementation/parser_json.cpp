#include "parser_json.h"

#include <unordered_map>
#include <list>

#include "iridium/parsing/implementation/node.h"
#include "iridium/convertion/convert.h"
#include "iridium/assert.h"
#include "iridium/strings.h"
#include "iridium/items.h"


using std::string;
using std::unordered_map;
using std::list;
using std::vector;
using iridium::convertion::convert;


namespace {


static const string DEFAULT_TAB = "    ";


}


#include "iridium/logging/logger.h"
namespace iridium {
namespace parsing {
namespace implementation {


// void convertStringToNode(string const &source, INode::TSharedPtr node, size_t &index, bool const &is_array_item = false) {
//     enum class TMarker {
//         NAME,
//         VALUE
//     };
//
//     string  name;
//     string  value;
//     TMarker marker      = TMarker::NAME;
//
//     auto clearEntry     = [&name, &value] () {
//         name.clear();
//         value.clear();
//     };
//     auto validateName   = [&name] () {
//         if (name.empty())
//             return; // ----->
//
//         if (name[0] >= '0' && name[0] <= '9')
//             throw std::runtime_error("json parsing error: wrong json field name: " + name); // ----->
//
//         for (auto const ch: name)
//             if (ch != '.' && ispunct(static_cast<int>(ch)))
//                 throw std::runtime_error("json parsing error: wrong json field name: " + name); // ----->
//     };
//     auto trimEntry      = [&name, &value] () {
//         name    = trim(name , " \t\n\r\"");
//         value   = trim(value, " \t\n\r\"");
//         if (name.substr(0, 2)  == "\\\"")
//             name  += "\"";
//         if (value.substr(0, 2) == "\\\"")
//             value += "\"";
//     };
//     auto addEntry       = [
//         &name,
//         &value,
//         &node,
//         &validateName,
//         &trimEntry,
//         &clearEntry,
//         &is_array_item
//     ] () {
//         trimEntry();
//         if (name.empty() || value.empty()) {
//            // array simple item
//             if (is_array_item && !name.empty() && value.empty())
//                 node->addChild("", name);
//             else {
//                // todo: error
//                throw std::runtime_error("json parsing error: wrong node for array item, node: '" +
//                    name + "' = '" + value + "'"); // ----->
//             }
//         } else {
//             if (name == "#text")
//                 name.clear();
//
//             validateName();
//             node->addChild(name, value);
//         }
//         clearEntry();
//     };
//
//     while (index < source.size()) {
//         auto ch  = source[index];
//         index++;
//
//         if (ch == '{') {
//             INode::TSharedPtr node_child;
//
//             trimEntry();
//             validateName();
//
//             if (name.empty())
//                 node_child = node;
//             else
//                 node_child = node->addChild(name, "");
//
//             convertStringToNode(source, node_child, index); // <-----
//
//             if (is_array_item)
//                 return; // <-----
//
//             clearEntry();
//             continue; // <---
//         }
//
//         if (ch == '[') {
//             while (ch != ']') {
//                 trimEntry();
//                 validateName();
//                 INode::TSharedPtr node_array_item_tmp = CNode::create(name);
//                 convertStringToNode(source, node_array_item_tmp, index, true); // <-----
//                 if (node_array_item_tmp->hasChilds()) {
//                     auto node_array_item = CNode::create(name);
//                     for (auto const &child: *node_array_item_tmp)
//                         if (child->getName().empty())
//                            // simple array item like [1, 2, ... ]
//                             node->addChild(name, child->getValue());
//                         else
//                            // object array item like [ { ... },  { ... }, ...]
//                             node_array_item->addChild(child);
//                     if (node_array_item->hasChilds())
//                         node->addChild(node_array_item);
//                 }
//                 ch = source[index - 1];
//             }
//
//             clearEntry();
//             continue; // <---
//         }
//
//         if (ch == ',') {
//             addEntry();
//             marker = TMarker::NAME;
//             continue; // <---
//         }
//
//         if (ch == '}' || ch == ']') {
//             addEntry();
//             return; // ----->
//         }
//
//         if (ch == ':') {
//             marker = TMarker::VALUE;
//             continue; // <---
//         }
//
//         if (marker == TMarker::NAME)
//             name  += ch;
//
//         if (marker == TMarker::VALUE)
//             value += ch;
//     }
// }


//bool checkOneOf(char const &symbol, string const &symbols) {
//    for (auto const &ch: symbols)
//        if (ch == symbol)
//            return true; // ----->
//    return false; // ----->
//}


//INode::TSharedPtr convertStringToNode(string const &source) {
//    INode::TSharedPtr           node = CNode::create("root");
//    INode::TSharedPtr           array_node;
//    vector<INode::TSharedPtr>   nodes{node}; // nodes stack
//    vector<char>                brackets; // expected brackets


//    string  name;
//    string  value;
//    size_t  index       = 0;
//    size_t  line        = 1;
//    size_t  position    = 0;
//    bool    is_masked   = false;

//    auto getErrorPos = [&line, &position]() {
//        return
//        "at line "      + convert<string>(line) +
//        " position "    + convert<string>(position);
//    };

//    auto throwUnexpectedSymbol = [&line, &position](char const &ch) {
//        throw std::runtime_error(string("json parsing error: unexpected '") + ch +
//            "' at line " + convert<string>(line) +
//            " position " + convert<string>(position)); // ----->
//    };

//    // find first '{'
//    for (index = 0; index < source.size(); index++) {
//        auto const &ch = source[index];

//        if (checkOneOf(ch, " \n\r\t"))
//            continue; // <---

//        if (ch == '{')
//            break; // --->
//        else
//            throwUnexpectedSymbol(ch); // ----->
//    }

//    index++;
//    brackets.push_back('}');
//    nodes.push_back(node);

//    auto root = node;
//    while (index < source.size()) {
//        char const &ch = source[index];
//        index++;

//        if (ch == '\n') {
//            line++;
//            position = 0;
//        } else
//            position++;

//        if (is_masked) {
//            value      += ch;
//            is_masked   = false;
//        } else {
//            if (ch == '\\') {
//                is_masked = true;
//                continue; // <---
//            }

//            // last bracket
//            char bracket = brackets.empty() ? 0 : brackets.back();

//            if (bracket != '"' && checkOneOf(ch, " \n\r\t"))
//                continue; // <-----

//            LOGT <<
//                 "\nch      = '" << string() + ch <<
//                "'\nname    = '" << name <<
//                "'\nvalue   = '" << value <<
//                "'\nbracket = '" << string() + bracket <<
//                "'\nbs      = "  << brackets.size() <<
//                 "\nns      = "  << nodes.size() <<
//                 "\n" << root;

//            if (ch == '"') {
//                if (bracket == '"')
//                    brackets.pop_back();
//                else
//                    brackets.push_back('"');
//                continue; // <---
//            }

//            if (ch == '{') {
//                brackets.push_back('}');
//                nodes.push_back(node);
//                if (!name.empty())
//                    node = node->addChild(name);
//                continue; // <---
//            }

//            if (ch == '[') {
//                brackets.push_back(']');
//                array_node = node;
//                continue; // <---
//            }

//            if (bracket == ']' && ch == ':')
//                throw std::runtime_error(string("json parsing error: expected '") +
//                    bracket + "' but got '" + ch + "' " + getErrorPos()); // ----->

//            if (checkOneOf(bracket, "}]")) {
//                if (ch == ':') {
//                    name = std::move(value);
//                    continue; // <---
//                }

//                if (checkOneOf(ch, "]},")) {
//                    if (brackets.size() > 1 && !name.empty()) {
//                        node->addChild(name, value);
//                        value.clear();
//                    }

//                    LOGT << "1 " << string() + bracket;
//                    if (ch == bracket) {
//                        brackets.pop_back();
//                        if (brackets.empty())
//                            bracket = 0;
//                        else
//                            bracket = brackets.back();
//                    }
//                    LOGT << "2 " << string() + bracket;

//                    if (ch == '}') {
//                        if (bracket == ']') {
//                            name.clear();

//                        } else {
//                            LOGT << "3 " << node->getName();
//                            nodes.pop_back();

//                            if (nodes.empty())
//                                throwUnexpectedSymbol('}'); // ----->

//                            node = nodes.back();

//                            node->addChild(name, value);

//                            name = node->getName();
//                        }

//                        LOGT << "4 " << node->getName();
//                    }

//                    continue; // <---
//                }
//            }

//            value += ch;
//        }
//    }

//    if (!brackets.empty())
//        throw std::runtime_error(string("json parsing error: expects '") +
//            brackets.back() + "' " + getErrorPos()); // ----->

//    return node; // ----->
//}

//INode::TSharedPtr convertJSONStringToNode2(string const &source) {
//    enum class TContext {
//        UNKNOWN,
//        SPACE,
//        KEY,
//        VALUE,
//        ARRAY,
//        OBJECT
//    };
//
//    struct TState {
//        TContext context;
//        struct TNext {
//            TContext context;
//            size_t   index;
//        }
//    };
//
//    //                                  0123456789012
//    static std::string const symbols = " \n\t{}[]:,\"";
//
//    std::vector< TState > states = {
//        { TContext::SPACE, TContext::SPACE },
//    };
//
//    auto state_index    = 0;
//    auto context        = TContext::SPACE;
//
//    for (auto const &ch: source) {
//        if (context == TContext::SPACE && checkOneOf(ch, ' ', '\t', '\n'))
//            continue; // <---
//
//        if (ch == states[state_index]) {
//            context =
//        }
//    }
//}


constexpr char unescapeChar(char ch) {
    switch (ch) {
        case 'n':  return '\n';   // newline
        case 't':  return '\t';   // tab
        case 'r':  return '\r';   // carriage return
        case 'b':  return '\b';   // backspace
        case 'f':  return '\f';   // form feed
        case 'v':  return '\v';   // vertical tab
        case 'a':  return '\a';   // alert (bell)
        case '\\': return '\\';   // backslash
        case '"':  return '"';    // double quote
        case '\'': return '\'';   // single quote
        case '0':  return '\0';   // null character
        default:   return ch;     // all other characters remain unchanged
    }
}


// todo: optimize with string_view
INode::TSharedPtr convertJSONStringToNode(string const &source) {
//    LOGT << source;

    // xml node without node name
    static string const DEFAULT_NODE_NAME_TEXT   = "#text";
    static string const DEFAULT_NODE_NAME_ROOT   = "root";
    static string const DEFAULT_NODE_NAME_ARRAY  = "array";

    INode::TSharedPtr       node = CNode::create(DEFAULT_NODE_NAME_ROOT), root = node;
    list<INode::TSharedPtr> stack;

    struct TArrayNode {
        INode::TSharedPtr   node;
        std::string         name;
    };

    list<TArrayNode>        stack_array;
    string                  expected_brackets;

    string  name;
    string  value;
    bool    is_quotes       = false;
    bool    is_quoted_value = false;
    bool    is_masked       = false;
    size_t  line            = 0;
    size_t  index           = 0;
    bool    is_expect_value = false;

    for (auto const &ch: source) {
        index++;
        if (ch == '\n') {
            index = 0;
            line++;
        }

        if (is_quotes) {
            if (is_masked) {
                is_masked = false;
                value += unescapeChar(ch);
                continue; // <---
            } else {
                if (ch == '\\') {
                    is_masked = true;
                    continue; // <---
                }

                if (ch == '"') {
                    is_quotes       = false;
                    is_quoted_value = true;
                    continue; // <---
                }
            }

            value += ch;
        } else {
            if (checkOneOf(ch, ' ', '\n', '\r', '\t'))
                continue; // <---

            if (ch == '"') {
                is_quotes       = true;
                is_quoted_value = true;
                continue; // <---
            }

            if (ch == ':') {
                if (value.empty()) {
                    throw std::runtime_error(
                        string("json parsing error: missing key before ':' at ") +
                        convert<string>(line) + ':' + convert<string>(index));
                }

                name = std::move(value);
                value.clear();

                if (name == DEFAULT_NODE_NAME_TEXT)
                    name.clear();

                is_quoted_value = false;
                continue; // <---
            }

            if (ch == '{') {
                is_expect_value = false;
                if (name.empty())
                    name = DEFAULT_NODE_NAME_ROOT;
//                LOGT << "push  node: " << node->getName() << " -> " << name;
                stack.push_back(node);
                node = node->addChild(name);
                expected_brackets.push_back('}');
//                LOGT << "open : " << string() + ch << " , brackets: '" << expected_brackets << "'";
                continue; // <---
            }

            if (ch == '[') {
                is_expect_value = false;
                if (name.empty())
                    name = DEFAULT_NODE_NAME_ARRAY;
//                LOGT << "push array: " << node->getName() << " -> " << name;
                stack_array.push_back( { node, name } );
                expected_brackets.push_back(']');
//                LOGT << "open : " << string() + ch << " , brackets: '" << expected_brackets << "'";
                continue; // <---
            }


            if (checkOneOf(ch, '}', ']', ',')) {
//                LOGT
//                    << line << ":" << index
//                    << "; ch = '" << string() + ch << "' "
//                    << "; name = '" << name << "' "
//                    << "; value = '" << value << "' "
//                    << "; is_quotes = '" << is_quoted_value << "' ";

                if (value.empty()) {
                    if (is_expect_value && !is_quoted_value) {
                        throw std::runtime_error(
                            string("json parsing error: missing value after ',' at ") +
                            convert<string>(line) + ':' +
                            convert<string>(index)
                        );
                    }
                } else {
                    static string const TRUE_   = "true";
                    static string const FALSE_  = "false";
                    static string const NULL_   = "null";

                    // true, false, int, float allowed without quotes
                    if (!is_quoted_value &&
                        !checkOneOf(value, TRUE_, FALSE_, NULL_) &&
                        // todo: check '-' on first symbol only
                        value.find_first_not_of("-0.123456789") != string::npos)
                    {
                        throw std::runtime_error(
                            string("json parsing error: unquoted value '") + value + "' at " +
                            convert<string>(line) + ':' +
                            convert<string>(index - value.size())); // ----->
                    }

                    node->addChild(name, value);
                    value.clear();
                }
                is_expect_value = ch == ',';

                bool is_expected_bracket = false;
                if (!expected_brackets.empty() &&
                    (expected_brackets.back() == ch))
                {
                    is_expected_bracket = true;
                    expected_brackets.pop_back();
//                    LOGT << "close: " << string() + ch << " , brackets: '" << expected_brackets << "'";
                }

                if ((ch == '}' && (stack.empty()        || !is_expected_bracket)) ||
                    (ch == ']' && (stack_array.empty()  || !is_expected_bracket)))
                {
                    throw std::runtime_error(
                        string("json parsing error: unxpected symbol '") +
                        ch + "' at " + convert<string>(line) + ':' + convert<string>(index)); // ----->
                }

                if (ch == '}') {
                    node = stack.back();
//                    LOGT << "pop   node: " << name << " -> " << node->getName();
                    stack.pop_back();
                    if (stack_array.empty() || stack_array.back().node != node)
                        name = node->getName();
                    else
                        name = stack_array.back().name;
                }

                if (ch == ']') {
                    node = stack_array.back().node;
//                    LOGT << "pop  array: " << name << " -> " << node->getName();
                    stack_array.pop_back();
                    name = node->getName();
                }

                continue; // <---
            }

            if (!name.empty()) {
                value += ch;
                continue; // <---
            }

            throw std::runtime_error(
                string("json parsing error: unxpected symbol '") +
                ch + "' at " + convert<string>(line) + ':' + convert<string>(index)); // ----->
        }
    }

    if (!expected_brackets.empty())
        throw std::runtime_error(string("json parsing error: expected symbol '") +
            expected_brackets.back() + "' at end of json"); // ----->

    if (root->size() == 1 && root->getChild(DEFAULT_NODE_NAME_ROOT))
        return root->getChild(DEFAULT_NODE_NAME_ROOT); // ----->
    else
        return root; // ----->
}


INode::TSharedPtr CJSONParser::parse(std::string const &source) const {
//     INode::TSharedPtr   root_node = CNode::create("root", "");
//     size_t              index = 0;
//
//     convertStringToNode(source, root_node, index);
//
//     if (root_node->size() == 1)
//         root_node = *root_node->begin();
//     else {
//         auto childs = root_node;
//         root_node = CNode::create("root");
//         root_node->addChild(childs);
//     }
//
//     return root_node; // ----->

    auto node = convertJSONStringToNode(source);
    if  (node) {
        if (node->size() == 1)
            node = *node->begin();
    } else
        throw std::runtime_error("json parsing error: node is null"); // ----->

    return node; // ----->
}


string mask(string const &source) {
    string result;
    for (auto const &ch: source) {
        if (ch == '"' || ch == '\\')
            result += '\\';
        result += ch;
    }
    return result; // ----->
}


void convertNodeToJSONString(INode::TConstSharedPtr const &node, string &result, string const &tab = "") {
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
                result += tab + "\"" +  node_child->getName() + "\"" + ": {\n";
                convertNodeToJSONString(node_child, result, tab + DEFAULT_TAB); // <-----
                result += tab + "}" + line_end;
            } else {
                string name = node_child->getName();

                if (name.empty())
                    name = "#text";

                result += tab;
                if (tab.empty())
                    result += DEFAULT_TAB;
                result += "\"" + name + "\"" + ": " + "\"" + mask(node_child->getValue()) + "\"" + line_end;
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
                    convertNodeToJSONString(node, result, tab + DEFAULT_TAB + DEFAULT_TAB); // <-----
                    result += tab + DEFAULT_TAB + "}" + line_end_;
                } else {
                    result += tab + DEFAULT_TAB + "\"" + mask(node->getValue()) + "\"" + line_end_;
                }
            }
            result += tab + "]" + line_end;
        }
    }
}


string CJSONParser::compose(INode::TConstSharedPtr const &root_node) const {
    string result;
    convertNodeToJSONString(root_node, result, DEFAULT_TAB + DEFAULT_TAB);
    result = "{\n" + DEFAULT_TAB + "\"" + root_node->getName() + "\"" + ": {\n" + result + DEFAULT_TAB + "}\n}\n";
    return result; // ----->
}


} // implementation
} // parser
} // iridium
