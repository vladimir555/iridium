// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "json_parser.h"


#include "iridium/parsing/implementation/node.h"
#include "iridium/convertion/convert.h"
#include "iridium/assert.h"
#include "iridium/strings.h"
#include "iridium/items.h"

#include <unordered_map>
#include <algorithm>
#include <string>
#include <string_view>
#include <list>
#include <stdexcept>


using std::string;
using std::string_view;
using std::unordered_map;
using std::list;
using std::vector;
using iridium::convertion::convert;


namespace {


static const string DEFAULT_TAB = "    ";


}


namespace iridium::parsing::implementation {


void encodeUTF8(uint32_t codepoint, std::string &out) {
    if (codepoint < 0x80) {
        out += static_cast<char>(codepoint);
    }

    else

    if (codepoint < 0x800) {
        out += static_cast<char>(0xC0 | (codepoint >> 6));
        out += static_cast<char>(0x80 | (codepoint & 0x3F));
    }

    else

    if (codepoint < 0x10000) {
        out += static_cast<char>(0xE0 |  (codepoint >> 12));
        out += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        out += static_cast<char>(0x80 |  (codepoint       & 0x3F));
    }

    else

    if (codepoint < 0x110000) {
        out += static_cast<char>(0xF0 |  (codepoint >> 18));
        out += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((codepoint >>  6) & 0x3F));
        out += static_cast<char>(0x80 |  (codepoint        & 0x3F));
    }
};


uint32_t convertHEXToCodepoint(char const *s) {
    uint32_t codepoint = 0;

    for (int i = 0; i < 4; ++i) {
        codepoint <<= 4;
        char c = s[i];

        if (c >= '0' && c <= '9')
            codepoint |= (c - '0');

        else

        if (c >= 'a' && c <= 'f')
            codepoint |= (c - 'a' + 10);

        else

        if (c >= 'A' && c <= 'F')
            codepoint |= (c - 'A' + 10);
    }

    return codepoint; // ----->
};


std::string unescape(std::string_view escaped_source) {
    std::string result;

    result.reserve(escaped_source.size());

    for (size_t i = 0; i < escaped_source.size(); ++i) {
        if (escaped_source[i] == '\\' && i + 1 < escaped_source.size()) {
            char next = escaped_source[++i];

            switch (next) {
                case 'n':
                    result += '\n';
                    break;
                case 'r':
                    result += '\r';
                    break;
                case 't':
                    result += '\t';
                    break;
                case '\\':
                    result += '\\';
                    break;
                case 'b':
                    result += '\b';
                    break;
                case 'f':
                    result += '\f';
                    break;
            }

            if (checkOneOf(next, '/', '"')) {
                result += next;
            }

            else

            if (next == 'u') {
                if (i + 4 >= escaped_source.size()) {
                    result += "\\u";
                    i--;
                    break;
                }

                uint32_t cp = convertHEXToCodepoint(escaped_source.data() + i + 1);
                i += 4;

                if (cp >= 0xD800 && cp <= 0xDBFF) {
                    if (i + 2 < escaped_source.size() &&
                        escaped_source[i + 1] == '\\' &&
                        escaped_source[i + 2] == 'u'  &&
                        i + 6 < escaped_source.size())
                    {
                        uint32_t low = convertHEXToCodepoint(escaped_source.data() + i + 3);

                        if (low >= 0xDC00 && low <= 0xDFFF) {
                            cp = 0x10000 + ((cp - 0xD800) << 10) + (low - 0xDC00);
                            i += 6;
                        }
                    }
                }

                encodeUTF8(cp, result);
            }

            else

            {
                // If an unknown escape character is encountered (for example, \x),
                // leave it as is (two characters)
                result += '\\';
                result += next;
            }
        } else {
            result += escaped_source[i];
        }
    }
    return result;
}


INode::TSharedPtr convertJSONStringToNode(std::string const &source) {
    static std::string const DEFAULT_NODE_NAME_TEXT  = "#text";
    static std::string const DEFAULT_NODE_NAME_ROOT  = "root";
    static std::string const DEFAULT_NODE_NAME_ARRAY = "array";

    enum class TState { NAME, VALUE };

    struct TStackItem {
        INode
           *node;
        TState
            state;
        char
            scope;
        std::string_view
            array_name;
        bool
            has_elements;
    };

    INode::TSharedPtr
        root_shared = CNode::create(DEFAULT_NODE_NAME_ROOT);
    INode
       *node = root_shared.get();
    size_t
        left = 0;
    size_t
        right = std::string::npos;
    auto
        const source_size = source.size();
    auto
        const source_data = source.data();
    std::vector<TStackItem>
        stack;

    stack.reserve(64);

    std::string_view
        name;
    std::string_view
        value;

    auto throwErrorUnexpectedSymbol = [&] () {
        char ch = (right < source_size) ? source_data[right] : ' ';

        std::string
            symbol = ch > 32 ?
            std::string(1, ch) :
            "\\x" + rjust(convert<std::string, uint32_t>(ch, 16), 2, '0');

        size_t context_start, context_end;

        if (right - left < 30) {
            context_start   = (left >= 10) ? (left - 10) : 0;
            context_end     = std::min(right + 20, source_size);
        } else {
            context_start   = left;
            context_end     = right + 1;
        }

        throw std::runtime_error(
            "json parsing error: unexpected symbol '" + symbol +
            "' on substring '" + source.substr(context_start, context_end - context_start) + "'");
    };

    do {
        left = (right == std::string::npos) ? 0 : right + 1;

        //checkIsSpace[(unsigned char)source_data[left]]
        while (left < source_size && checkOneOf(source_data[left], ' ', '\t', '\r', '\n'))
            left++;

        right = left;

        if (right >= source_size)
            break;

        if(!stack.empty() && stack.back().has_elements &&
            stack.back().state == TState::NAME &&
            source_data[right] == '}')
        {
            throwErrorUnexpectedSymbol();
        }

        if (checkOneOf(source_data[right], '{', '[')) {
            if (name.empty() && !stack.empty() && stack.back().scope == '}')
                throwErrorUnexpectedSymbol();

            if (source_data[right] == '{') {
                if (!name.empty()) {
                    node = node->addChild(std::string(name)).get();
                    name = std::string_view();
                }

                else

                if (!stack.empty() && stack.back().scope == ']' && !stack.back().array_name.empty()) {
                    node = node->addChild(std::string(stack.back().array_name)).get();
                }

                else

                if (stack.empty()) {
                    node = node->addChild(DEFAULT_NODE_NAME_ROOT).get();
                }
            } else {
                if (stack.empty() && name.empty()) {
                    node = node->addChild(DEFAULT_NODE_NAME_ROOT).get();
                    name = DEFAULT_NODE_NAME_ARRAY;
                }
            }

            stack.push_back(TStackItem {
                node,
                source_data[right] == '{' ? TState::NAME : TState::VALUE,
                source_data[right] == '{' ? '}' : ']',
                source_data[right] == '[' ? name : std::string_view(),
                false
            });
            continue;
        }

        if (stack.empty())
            throwErrorUnexpectedSymbol();

        if (checkOneOf(source_data[right], '}', ']')) {
            if (stack.back().scope != source_data[right])
                throwErrorUnexpectedSymbol();

            stack.pop_back();

            if (!stack.empty()) {
                node = stack.back().node;
                stack.back().has_elements = true;
            }

            name    = std::string_view();
            value   = std::string_view();

            continue;
        }

        if (source_data[right] == ':') {
            if (stack.back().state == TState::NAME)
                stack.back().state = TState::VALUE;
            else
                throwErrorUnexpectedSymbol();

            continue;
        }

        if (source_data[right] == ',') {
            stack.back().state = (stack.back().scope == '}') ? TState::NAME : TState::VALUE;
            continue;
        }

        if (source_data[right] == '"') {
            size_t  string_start = right;
            bool    is_escaped = false;
            size_t  value_start_pos = right + 1;

            right = value_start_pos;

            while (right < source_size) {
                while (right < source_size && !checkOneOf(source_data[right], '"', '\\', '\x00')) {
                    right++;
                }

                if (right >= source_size || source_data[right] == '\x00')
                    break;

                if (source_data[right] == '\\') {
                    is_escaped = true;
                    right += 2;
                    continue;
                }

                value = std::string_view(source_data + value_start_pos, right - value_start_pos);

                if (stack.back().state == TState::NAME) {
                    // todo: escaped names
                    if (is_escaped) {
                        left = string_start;
                        throwErrorUnexpectedSymbol();
                    }
                    name = (value == "#text") ? "" : value;
                }

                else

                if (stack.back().state == TState::VALUE) {
                    if (name.empty() && stack.back().scope == ']')
                        name = stack.back().array_name.empty() ? DEFAULT_NODE_NAME_ARRAY : stack.back().array_name;

                    if (is_escaped)
                        node->addChild(std::string(name), unescape(value));
                    else
                        node->addChild(std::string(name), std::string(value));

                    name = std::string_view();
                    stack.back().has_elements = true;
                }

                break;
            }

            continue;
        }

        if (stack.back().state == TState::VALUE) {
            left = right;
            while (right < source_size && !checkOneOf(source_data[right], ' ', '\t', '\r', '\n', '}', ']', ','))
                right++;

            value = std::string_view(source_data + left, right - left);

            if (checkOneOf(value, "null", "false", "true") ||
                value.find_first_not_of("-0123456789.eE+") == std::string::npos)
            {
                if (name.empty() && stack.back().scope == ']')
                    name = stack.back().array_name.empty() ? DEFAULT_NODE_NAME_ARRAY : stack.back().array_name;

                node->addChild(std::string(name), std::string(value));
                name = std::string_view();
                stack.back().has_elements = true;

                if (right < source_size) {
                    char ch = source_data[right];
                    if (ch == '}' || ch == ']' || ch == ',')
                        right--;
                }
            } else {
                throwErrorUnexpectedSymbol();
            }
        } else {
            throwErrorUnexpectedSymbol();
        }
    } while (right < source_size);

    if (stack.empty()) {
        if (root_shared->size() == 1 && root_shared->getChild(DEFAULT_NODE_NAME_ROOT))
            return root_shared->getChild(DEFAULT_NODE_NAME_ROOT);
        return root_shared;
    } else {
        throw std::runtime_error("json parsing error: expected scope '" + std::string(1, stack.back().scope) + "' at end of json");
    }
}


INode::TSharedPtr CJSONParser::parse(std::string const &source) const {
    auto node = convertJSONStringToNode(source);
    if  (node) {
        if (node->size() == 1)
            node = *node->begin();
    } else
        throw std::runtime_error("json parsing error: node is null"); // ----->

    return node; // ----->
}


inline void appendEscaped(std::string& result, std::string_view value) {
    for (char c : value) {
        if (c == '"') {
            result += "\\\"";
        } else if (c == '\\') {
            result += "\\\\";
        } else {
            result += c;
        }
    }
}


void convertNodeToJSONString(INode::TConstSharedPtr const &node, std::string &result, std::string const &tab = "") {
    std::unordered_map<std::string, std::list<INode::TConstSharedPtr>>
        map_name_node;
    std::vector<std::string>
        names;

    map_name_node.reserve(node->size());
    names.reserve(node->size());

    for (auto const &i : *node) {
        auto const &name = i->getName();

        if (map_name_node.find(name) == map_name_node.end())
            names.push_back(name);

        map_name_node[name].push_back(i);
    }

    size_t name_number = 0;
    for(auto const &name: names) {
        auto const &nodes = map_name_node[name];
        std::string line_end;

        name_number++;
        if (map_name_node.size() == 1 || name_number == names.size())
            line_end = "\n";
        else
            line_end = ",\n";

        if (nodes.size() == 1) {
            auto node_child = *nodes.begin();

            if (node_child->hasChilds()) {
                result += tab;
                result += '"';
                result += node_child->getName();
                result += "\": {\n";

                convertNodeToJSONString(node_child, result, tab + DEFAULT_TAB);

                result += tab;
                result += '}';
                result += line_end;
            } else {
                auto name_ = node_child->getName();

                if (name_.empty())
                    name_ = "#text";

                result += tab;

                if (tab.empty())
                    result += DEFAULT_TAB;

                result += '"';
                result += name_;
                result += "\": \"";

                appendEscaped(result, node_child->getValue());

                result += '"';
                result += line_end;
            }
        } else {
            result += tab;
            result += '"';
            result += name;
            result += "\": [\n";

            size_t node_number = 0;

            for (auto const &node_ : nodes) {
                std::string line_end_;

                node_number++;

                if (node_number == nodes.size())
                    line_end_ = "\n";
                else
                    line_end_ = ",\n";

                if (node_->hasChilds()) {
                    result += tab;
                    result += DEFAULT_TAB;
                    result += "{\n";

                    convertNodeToJSONString(node_, result, tab + DEFAULT_TAB + DEFAULT_TAB);

                    result += tab;
                    result += DEFAULT_TAB;
                    result += '}';
                    result += line_end_;
                } else {
                    result += tab;
                    result += DEFAULT_TAB;
                    result += '"';

                    appendEscaped(result, node_->getValue());

                    result += '"';
                    result += line_end_;
                }
            }
            result += tab;
            result += ']';
            result += line_end;
        }
    }
}


string CJSONParser::compose(INode::TConstSharedPtr const &root_node) const {
    string result;

    convertNodeToJSONString(root_node, result, DEFAULT_TAB + DEFAULT_TAB);

    result = "{\n" + DEFAULT_TAB + "\"" + root_node->getName() + "\"" + ": {\n" + result + DEFAULT_TAB + "}\n}\n";

    return result; // ----->
}


} // iridium::parsing::implementation
