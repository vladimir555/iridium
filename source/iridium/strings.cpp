#include "strings.h"

#include <algorithm>
#include <locale>


using std::string;
using std::vector;
using std::list;
using std::transform;


namespace iridium {


list<string> split(string const &source, string const &delimiter, size_t const &max_items) {
    list<string> result;
    size_t lpos = 0, rpos = 0;
    while (rpos != string::npos) {

        if (max_items == 0 || result.size() < (max_items - 1))
            rpos = source.find(delimiter, lpos + delimiter.size() - 1);
        else
            rpos = string::npos;

        if (lpos < rpos) {
            auto item = source.substr(lpos, rpos - lpos);
            if (item != delimiter && !item.empty())
                result.push_back(item);
        }

        lpos = rpos + delimiter.size();
    }
    return result; // ----->
}


list<string> split(string const &source, string const &delimiter) {
    if (delimiter.empty())
        throw std::runtime_error("empty delimiter"); // ----->
    return split(source, delimiter, 0);
}


string lowerCase(string const &source) {
    string result = source;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result; // ----->
}


string upperCase(string const &source) {
    string result = source;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result; // ----->
}


string replace(string const &source, string const &from, string const &to) {
    size_t lpos = 0;
    size_t rpos = 0;
    string result;

    while (rpos != string::npos) {
        rpos = source.find(from, lpos);
        if (rpos == string::npos)
            result += source.substr(lpos, string::npos);
        else {
            result += source.substr(lpos, rpos - lpos);
            result += to;
        }
        lpos = rpos + from.size();
    }
    return result; // ----->
}


string rjust(string const &source, size_t const &width, char const &symbol) {
    if (width > source.size())
        return string(width - source.size(), symbol) + source; // ----->
    else
        return std::move(source); // ----->
}


string ljust(string const &source, size_t const &width, char const &symbol) {
    if (width > source.size())
        return source + string(width - source.size(), symbol); // ----->
    else
        return std::move(source); // ----->
}


string trim(string const &source, string const &symbols) {
    if (source.empty())
        return ""; // ----->

    size_t lpos = 0;
    size_t rpos = source.size() - 1;

    auto testSymbol = [symbols] (char const &ch_) -> bool {
        for (auto const ch : symbols)
            if (ch == ch_)
                return true; // --->
        return false;
    };

    while (lpos <= rpos && testSymbol(source[lpos]))
        lpos++;

    while (lpos <= rpos && testSymbol(source[rpos]))
        rpos--;

    if (rpos >= lpos)
        return source.substr(lpos, rpos - lpos + 1); // ----->
    else
        return ""; // ----->
}


string trim(string const &source) {
    return trim(source, " \t\r\n"); // ----->
}


} // iridium
