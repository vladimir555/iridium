#include "strings.h"
#include <algorithm>
#include <locale>


using std::string;
using std::vector;
using std::transform;


namespace utility {


vector<string> split(string const &source, string const &delimiter) {
    vector<string> result;
    size_t lpos = 0, rpos = 0;
    while (rpos != string::npos) {
        rpos = source.find(delimiter, lpos + delimiter.size());
        auto item = source.substr(lpos, rpos - lpos);
        if (item != delimiter)
            result.push_back(item);
        lpos = rpos + delimiter.size();
    }
    return result; // ----->
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


string replace(string const &source, char const &from, char const &to) {
    string result = source;
    for (auto &ch: result)
        if (ch == from)
            ch =  to;
    return result; // ----->
}


string rjust(string const &source, size_t const &width, char const &symbol) {
    if (width > source.size())
        return string(width - source.size(), symbol) + source; // ----->
    else
        return std::move(source); // ----->
}


string trim(string const &source, string const &symbols) {
    if (source.empty())
        return ""; // ----->

    size_t left     = 0;
    size_t right    = source.size() - 1;

    auto testSymbol = [symbols] (char const &ch_) -> bool {
        for (auto const ch : symbols)
            if (ch == ch_)
                return true; // --->
        return false;
    };

    while (left <= right && testSymbol(source[left]))
        left++;

    while (left <= right && testSymbol(source[right]))
        right--;

    if (right >= left)
        return source.substr(left, right - left + 1); // ----->
    else
        return ""; // ----->
}


string trim(string const &source) {
    return trim(source, " \t\r\n"); // ----->
}


} // utility
