#include <string>
#include <vector>


namespace utility {

///
std::vector<std::string> split(std::string const &source, std::string const &delimiter);
///
std::string lowerCase(std::string const &source);
///
std::string upperCase(std::string const &source);
/// replace symbols 'from' to symbols 'to' in string 'source'
std::string replace(std::string const &source, char const &from, char const &to);
///
std::string rjust(std::string const &source, size_t const &width, char const &symbol);
/// remove symbols from begin and end of string
std::string trim(std::string const &source, std::string const &symbols);
/// remove symbols ' \t\n\r' from begin and end of string
std::string trim(std::string const &source);


} // utility
