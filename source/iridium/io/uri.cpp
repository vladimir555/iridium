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
    std::array<uint8_t , 4>{ 0 }
{}


string extractTokens(
    std::string const &source,
    std::string const &delimiter,
    std::string       &argument) 
{
    if (source.empty())
        return {};

    auto tokens = split(source, delimiter);

    if (tokens.size() >  2)
        throw std::runtime_error("too many '" + delimiter + "' found in '" + source + "'"); // ----->

    if (tokens.size() == 2)
        argument = tokens.front();

    return tokens.back();
}


URI::URI(std::string const &source)
:
    m_source(source)
{
    //string const IPv4_REGEX                = "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$";
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
        string source;
        string protocol;

        // protocol + user:pass@host:port/path?args
        // protocol + path/host args
        source = extractTokens(m_source, PROTOCOL_DELIMITER, protocol);
        if (protocol.empty())
            m_protocol = TProtocol::FILE;
        else
            m_protocol = convert<TProtocol>(protocol);

        if (checkOneOf(m_protocol, TProtocol::PROCESS, TProtocol::FILE)) {
            // path/host + args
            m_arguments = extractTokens(source, PROCESS_ARGUMENT_DELIMITER, m_address);
            if (m_address.empty())
                m_address = std::move(m_arguments);
            auto tokens = split(m_address, PROCESS_ARGUMENT_DELIMITER);
            m_path = m_address = tokens.front();
            if (tokens.size() == 2)
                m_arguments = tokens.back();
            
            m_host = split(m_path, PATH_DELIMITER).back();
        } else {
            string login;
            // user:pass + host:port/path?args
            source = extractTokens(source, LOGIN_DELIMITER, login);
            if (!login.empty()) {
                // user + pass
                m_password = extractTokens(login, USER_DELIMITER, m_user);
                if (m_user.empty())
                    m_user = std::move(m_password);
            }

            // host:port + path?args
            {
                auto tokens = split(source, PATH_DELIMITER, 2);
                m_address   = tokens.front();
                if (tokens.size() == 2)
                    source  = "/" + tokens.back();
            }

            if (m_address.empty())
                throw std::runtime_error("address is empty");
            else {
                // host + port
                string port = extractTokens(m_address, PORT_DELIMITER, m_host);
                if (m_host.empty()) {
                    m_port = m_protocol;
                    m_host = port;
                } else
                    m_port = convert<uint16_t>(port);
            }
            
            // path + args
            m_arguments = extractTokens(source, WEB_ARGUMENT_DELIMITER, m_path);
            if (m_path.empty())
                m_path = std::move(m_arguments);
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
