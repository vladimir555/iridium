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


// constexpr char unescapeChar(char ch) {
//     switch (ch) {
//         case 'n':  return '\n';   // newline
//         case 't':  return '\t';   // tab
//         case 'r':  return '\r';   // carriage return
//         case 'b':  return '\b';   // backspace
//         case 'f':  return '\f';   // form feed
//         case 'v':  return '\v';   // vertical tab
//         case 'a':  return '\a';   // alert (bell)
//         case '\\': return '\\';   // backslash
//         case '"':  return '"';    // double quote
//         case '\'': return '\'';   // single quote
//         case '0':  return '\0';   // null character
//         default:   return ch;     // all other characters remain unchanged
//     }
// }


// // parsing     time is 170ms719us
// INode::TSharedPtr convertJSONStringToNode(string const &source) {
// //    LOGT << source;

//     // xml text node without node name
//     static string const DEFAULT_NODE_NAME_TEXT   = "#text";
//     static string const DEFAULT_NODE_NAME_ROOT   = "root";
//     static string const DEFAULT_NODE_NAME_ARRAY  = "array";

//     INode::TSharedPtr       node = CNode::create(DEFAULT_NODE_NAME_ROOT), root = node;
//     list<INode::TSharedPtr> stack;

//     struct TArrayNode {
//         INode::TSharedPtr   node;
//         std::string         name;
//     };

//     list<TArrayNode>        stack_array;
//     string                  expected_brackets;

//     string  name;
//     string  value;
//     bool    is_quotes       = false;
//     bool    is_quoted_value = false;
//     bool    is_masked       = false;
//     size_t  line            = 0;
//     size_t  index           = 0;
//     bool    is_expect_value = false;

//     for (auto const &ch: source) {
//         index++;
//         if (ch == '\n') {
//             index = 0;
//             line++;
//         }

//         if (is_quotes) {
//             if (is_masked) {
//                 is_masked = false;
//                 value += unescapeChar(ch);
//                 continue; // <---
//             } else {
//                 if (ch == '\\') {
//                     is_masked = true;
//                     continue; // <---
//                 }

//                 if (ch == '"') {
//                     is_quotes       = false;
//                     is_quoted_value = true;
//                     continue; // <---
//                 }
//             }

//             value += ch;
//         } else {
//             if (checkOneOf(ch, ' ', '\n', '\r', '\t'))
//                 continue; // <---

//             if (ch == '"') {
//                 is_quotes       = true;
//                 is_quoted_value = true;
//                 continue; // <---
//             }

//             if (ch == ':') {
//                 if (value.empty()) {
//                     throw std::runtime_error(
//                         string("json parsing error: missing key before ':' at ") +
//                         convert<string>(line) + ':' + convert<string>(index));
//                 }

//                 name = std::move(value);
//                 value.clear();

//                 if (name == DEFAULT_NODE_NAME_TEXT)
//                     name.clear();

//                 is_quoted_value = false;
//                 continue; // <---
//             }

//             if (ch == '{') {
//                 is_expect_value = false;
//                 if (name.empty())
//                     name = DEFAULT_NODE_NAME_ROOT;
// //                LOGT << "push  node: " << node->getName() << " -> " << name;
//                 stack.push_back(node);
//                 node = node->addChild(name);
//                 expected_brackets.push_back('}');
// //                LOGT << "open : " << string() + ch << " , brackets: '" << expected_brackets << "'";
//                 continue; // <---
//             }

//             if (ch == '[') {
//                 is_expect_value = false;
//                 if (name.empty())
//                     name = DEFAULT_NODE_NAME_ARRAY;
// //                LOGT << "push array: " << node->getName() << " -> " << name;
//                 stack_array.push_back( { node, name } );
//                 expected_brackets.push_back(']');
// //                LOGT << "open : " << string() + ch << " , brackets: '" << expected_brackets << "'";
//                 continue; // <---
//             }


//             if (checkOneOf(ch, '}', ']', ',')) {
// //                LOGT
// //                    << line << ":" << index
// //                    << "; ch = '" << string() + ch << "' "
// //                    << "; name = '" << name << "' "
// //                    << "; value = '" << value << "' "
// //                    << "; is_quotes = '" << is_quoted_value << "' ";

//                 if (value.empty()) {
//                     if (is_expect_value && !is_quoted_value) {
//                         throw std::runtime_error(
//                             string("json parsing error: missing value after ',' at ") +
//                             convert<string>(line) + ':' +
//                             convert<string>(index)
//                         );
//                     }
//                 } else {
//                     static string const TRUE_   = "true";
//                     static string const FALSE_  = "false";
//                     static string const NULL_   = "null";

//                     // true, false, int, float allowed without quotes
//                     if (!is_quoted_value &&
//                         !checkOneOf(value, TRUE_, FALSE_, NULL_) &&
//                         // todo: check '-' on first symbol only
//                         value.find_first_not_of("-0.123456789") != string::npos)
//                     {
//                         throw std::runtime_error(
//                             string("json parsing error: unquoted value '") + value + "' at " +
//                             convert<string>(line) + ':' +
//                             convert<string>(index - value.size())); // ----->
//                     }

//                     node->addChild(name, value);
//                     value.clear();
//                 }
//                 is_expect_value = ch == ',';

//                 bool is_expected_bracket = false;
//                 if (!expected_brackets.empty() &&
//                     (expected_brackets.back() == ch))
//                 {
//                     is_expected_bracket = true;
//                     expected_brackets.pop_back();
// //                    LOGT << "close: " << string() + ch << " , brackets: '" << expected_brackets << "'";
//                 }

//                 if ((ch == '}' && (stack.empty()        || !is_expected_bracket)) ||
//                     (ch == ']' && (stack_array.empty()  || !is_expected_bracket)))
//                 {
//                     throw std::runtime_error(
//                         string("json parsing error: unxpected symbol '") +
//                         ch + "' at " + convert<string>(line) + ':' + convert<string>(index)); // ----->
//                 }

//                 if (ch == '}') {
//                     node = stack.back();
// //                    LOGT << "pop   node: " << name << " -> " << node->getName();
//                     stack.pop_back();
//                     if (stack_array.empty() || stack_array.back().node != node)
//                         name = node->getName();
//                     else
//                         name = stack_array.back().name;
//                 }

//                 if (ch == ']') {
//                     node = stack_array.back().node;
// //                    LOGT << "pop  array: " << name << " -> " << node->getName();
//                     stack_array.pop_back();
//                     name = node->getName();
//                 }

//                 continue; // <---
//             }

//             if (!name.empty()) {
//                 value += ch;
//                 continue; // <---
//             }

//             throw std::runtime_error(
//                 string("json parsing error: unxpected symbol '") +
//                 ch + "' at " + convert<string>(line) + ':' + convert<string>(index)); // ----->
//         }
//     }

//     if (!expected_brackets.empty())
//         throw std::runtime_error(string("json parsing error: expected symbol '") +
//             expected_brackets.back() + "' at end of json"); // ----->

//     if (root->size() == 1 && root->getChild(DEFAULT_NODE_NAME_ROOT))
//         return root->getChild(DEFAULT_NODE_NAME_ROOT); // ----->
//     else
//         return root; // ----->
// }


void encodeUTF8(uint32_t cp, std::string &out) {
    if (cp < 0x80) {
        out += static_cast<char>(cp);
    }

    else

    if (cp < 0x800) {
        out += static_cast<char>(0xC0 | (cp >> 6));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    }

    else

    if (cp < 0x10000) {
        out += static_cast<char>(0xE0 |  (cp >> 12));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 |  (cp       & 0x3F));
    }

    else

    if (cp < 0x110000) {
        out += static_cast<char>(0xF0 |  (cp >> 18));
        out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((cp >>  6) & 0x3F));
        out += static_cast<char>(0x80 |  (cp        & 0x3F));
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

            if (checkOneOf(next, 'n', 'r', 't', '\\', 'b', 'f')) {
                // Switch превращает букву в управляющий символ!
                switch (next) {
                    case 'n':  result += '\n'; break; // Буква 'n' -> символ переноса
                    case 'r':  result += '\r'; break; // Буква 'r' -> символ возврата каретки
                    case 't':  result += '\t'; break; // Буква 't' -> символ табуляции
                    case '\\': result += '\\'; break; // Буква '\' -> символ '\'
                    case 'b':  result += '\b'; break; // Буква 'b' -> символ backspace
                    case 'f':  result += '\f'; break; // Буква 'f' -> символ form feed
                }
            }
            else if (checkOneOf(next, '/', '"')) {
                // Здесь просто добавляем сам символ один раз
                result += next;
            }
            else if (next == 'u') {
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
            } else {
                // Если встретился неизвестный экранированный символ (например, \x),
                // оставляем его как есть (два символа)
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
            // !checkIsDelimiter[(unsigned char)source_data[right]]
            while (right < source_size && !checkOneOf(source_data[right], ' ', '\t', '\r', '\n', '}', ']', ','))
                right++;

            value = std::string_view(source_data + left, right - left);

            if (value == "null" || value == "false" || value == "true" ||
                value.find_first_not_of("-0123456789.eE+") == std::string::npos) {
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
                auto name_ = node_child->getName();

                if (name_.empty())
                    name_ = "#text";

                result += tab;
                if (tab.empty())
                    result += DEFAULT_TAB;
                result += "\"" + name_ + "\"" + ": " + "\"" + mask(node_child->getValue()) + "\"" + line_end;
            }
        } else {
            result += tab + "\"" + name + "\"" + ": [\n";
            size_t node_number = 0;
            for (auto const &node_ : nodes) {
                string line_end_;
                node_number++;
                if (nodes.size() == 1 || node_number == nodes.size())
                    line_end_ = "\n";
                else
                    line_end_ = ",\n";

                if (node_->hasChilds()) {
                    result += tab + DEFAULT_TAB + "{\n";
                    convertNodeToJSONString(node_, result, tab + DEFAULT_TAB + DEFAULT_TAB); // <-----
                    result += tab + DEFAULT_TAB + "}" + line_end_;
                } else {
                    result += tab + DEFAULT_TAB + "\"" + mask(node_->getValue()) + "\"" + line_end_;
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


} // iridium::parsing::implementation
