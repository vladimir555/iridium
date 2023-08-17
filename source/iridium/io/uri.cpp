#include "uri.h"

#include "iridium/strings.h"
#include "iridium/assert.h"
#include "iridium/items.h"
#include "net/dns.h"


using std::string;
using std::vector;
using iridium::convertion::convert;
using iridium::convertion::convertPtr;


IMPLEMENT_ENUM(iridium::io::URI::TProtocol)


namespace iridium {
namespace io {


URI::TIPv4::TIPv4()
:
    std::array<uint8_t, 4>{ 0 }
{}


string unmask(string const &source) {
    string  result;
    uint8_t code = 0;
    uint8_t masked_symbol_count = 0;

    for (auto const &ch : source) {
        static char const MASK_SYMBOL = '%';

        if (masked_symbol_count == 0) {
            if (ch == MASK_SYMBOL)
                masked_symbol_count++;
            else
                result += ch;
        } else {
            if (masked_symbol_count < 3) {
                code = (code << 4) + ch - '0';
                masked_symbol_count++;
            } else {
                result += code;
                code = 0;
                masked_symbol_count = 0;
                result += ch;
            }
        }
    }

    if(masked_symbol_count > 0)
        result += code;

    return result;
}


void extractTokens(
    std::string const &delimiter,
    std::string       &source,
    std::string       &argument) 
{
    if (source.empty())
        return;

    auto pos =  source.find(delimiter);
    if  (pos == string::npos)
        return;

    if (source.find(delimiter, pos + delimiter.size()) != string::npos)
        throw std::runtime_error("too many '" + delimiter + "' found in '" + source + "'"); // ----->

    argument = source.substr(0, pos);

    if (argument.empty())
        throw std::runtime_error("empty argument in '" + source + "'"); // ----->

    source   = source.substr(pos + delimiter.size());
}


URI::URI(std::string const &source)
:
    m_source(source)
{
    // string const IPv4_REGEX = "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$";
    static string const LOGIN_DELIMITER             = "@";
    static string const USER_DELIMITER              = ":";
    static string const PROTOCOL_DELIMITER          = "://";
    static string const PORT_DELIMITER              = ":";
    static string const PROCESS_ARGUMENT_DELIMITER  = " ";
    static string const WEB_ARGUMENT_DELIMITER      = "?";
    static string const PATH_DELIMITER              = "/";

    if (m_source.empty())
        throw std::runtime_error("uri '" + m_source +
            "' parsing error: empty"); // ----->

    try {
        string source = m_source;
        string protocol;

        // protocol + user:pass@host:port/path?args
        // protocol + path/host args
        extractTokens(PROTOCOL_DELIMITER, source, protocol);
        if (protocol.empty())
            m_protocol = TProtocol::FILE;
        else
            m_protocol = convert<TProtocol>(protocol);

        if (checkOneOf(m_protocol, TProtocol::PROCESS, TProtocol::FILE)) {
            auto tokens = split(source, PROCESS_ARGUMENT_DELIMITER, 2);

            if (m_protocol == TProtocol::FILE && tokens.size() > 1)
                throw std::runtime_error("wrong file path format");

            if (!tokens.empty())
                m_address   = tokens.front();

            if (!tokens.empty())
                m_arguments = tokens.back();

            // todo: windows path delimeter
            // address=path/host + args
            if (m_address.empty())
                m_address = unmask(source);
            else
                m_address = unmask(m_address);

            // todo: check file or dir
            auto pos =   m_address.find_last_of(PATH_DELIMITER);
            if  (pos == string::npos)
                m_host = m_address;
            else
                m_host = m_address.substr(pos + PATH_DELIMITER.size());

            m_path = m_address;
        } else {
            string login;
            // user:pass + host:port/path?args
            extractTokens(LOGIN_DELIMITER, source, login);

            if (!login.empty()) {
                // user + password
                extractTokens(USER_DELIMITER, login, m_user);
                if (m_user.empty())
                    m_user      = unmask(login);
                else {
                    m_user      = unmask(m_user);
                    m_password  = unmask(login);
                }
            }

            // host:port + path?args
            auto pos = source.find(PATH_DELIMITER);
            if (pos == string::npos) {
                m_address   = unmask(source);
            } else {
                m_address   = unmask(source.substr(0, pos));
                source      = source.substr(pos);
                // path + args

                extractTokens(WEB_ARGUMENT_DELIMITER, source, m_path);
                if (m_path.empty())
                    m_path      = unmask(source);
                else {
                    m_path      = unmask(m_path);
                    m_arguments = unmask(source);
                }
            }

            if (m_address.empty())
                throw std::runtime_error("address is empty");

            // host + port
            auto address = m_address;
            extractTokens(PORT_DELIMITER, address, m_host);
            if (m_host.empty()) {
                m_host = unmask(address);
                m_port = m_protocol;
            } else
                m_port = convert<uint16_t>(address);
        }

    } catch (std::exception const &e) {
        throw std::runtime_error("uri '" + m_source + "' parsing error: " + e.what());
    }
}


URI::TIPv4::TConstSharedPtr URI::getIPv4() const {
    if(!m_ipv4)
        m_ipv4 = std::make_shared<TIPv4>(net::getIPv4ByHost(m_host)); // ----->
    
    return m_ipv4;
}


URI::TIPv6::TConstSharedPtr URI::getIPv6() const {
    // todo:
    return {};
}


URI::TProtocol URI::getProtocol() const {
    return m_protocol; // ----->
}


std::string URI::getUser() const {
    return m_user; // ----->
}


std::string URI::getPassword() const {
    return m_password; // ----->
}


std::string URI::getHost() const {
    return m_host; // ----->
}


URI::TPort URI::getPort() const {
    return m_port; // ----->
}


std::string URI::getPath() const {
    return m_path; // ----->
}


std::string URI::getArguments() const {
    return m_arguments;
}


std::string URI::getAddress() const {
    return m_address; // ----->
}


std::string URI::getSource() const {
    return m_source;
}


bool URI::operator == (URI const &uri) const {
    return m_source == uri.m_source; // ----->
}


bool URI::operator <  (URI const &uri) const {
    return m_source < uri.m_source;
}


} // io
} // iridium


namespace {


iridium::io::URI createURI(string const &value) {
    return iridium::io::URI(value); // ----->
}


string getAddress(iridium::io::URI const &uri) {
    return uri.getSource(); // ----->
}


string convertIPv4ToString(iridium::io::URI::TIPv4 const &ipv4) {
    return
        convert<string>(ipv4[0]) + "." +
        convert<string>(ipv4[1]) + "." +
        convert<string>(ipv4[2]) + "." +
        convert<string>(ipv4[3]);
    // ----->
}


iridium::io::URI::TIPv4 convertStringToIPv4(string const &ipv4) {
    using iridium::assertSize;
    using iridium::assign;

    try {
        iridium::io::URI::TIPv4 result;
        auto const bytes = iridium::split(ipv4, ".");
        size_t i = 0;
        for (auto const &byte: assertSize(bytes, 4, "convertion string '" + ipv4 + "' to ipv4 error: expected 4 numbers"))
            result[i++] = convert<uint8_t>(byte);
        return result; // ----->
    } catch (std::exception const &e) {
        throw std::runtime_error("ipv4 '" + ipv4 + "' parsing error: " + e.what()); // ----->
    }
}


} // unnamed


IMPLEMENT_CONVERT(iridium::io::URI::TIPv4, std::string, convertStringToIPv4)
IMPLEMENT_CONVERT(std::string, iridium::io::URI::TIPv4, convertIPv4ToString)


IMPLEMENT_CONVERT(iridium::io::URI, std::string, createURI)
IMPLEMENT_CONVERT(std::string, iridium::io::URI, getAddress)
