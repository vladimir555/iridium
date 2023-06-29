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


URI::URI(std::string const &source)
:
    m_source(source)
{
    //string const IPv4_REGEX                = "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$";
    static string const PROTOCOL_DELIMITER = "://";
    static string const PORT_DELIMITER     = ":";

    if (m_source.empty())
        throw std::runtime_error("uri '" + m_source +
            "' parsing error: empty"); // ----->

    auto tokens = assign(split(m_source, PROTOCOL_DELIMITER, 2));

    if (tokens.size() > 2)
        throw std::runtime_error("uri '" + m_source +
            "' parsing error: too many '" + PROTOCOL_DELIMITER + "' found"); // ----->

    string address_port;
    if (tokens.size() == 2) {
        m_protocol = convert<TProtocol>(tokens[0]);
        if (checkOneOf(m_protocol, TProtocol::FILE, TProtocol::PROCESS)) {
            m_address = tokens[1];
            if (m_protocol == TProtocol::PROCESS) {
                tokens      = assign(split(m_address, " ", 2));
                m_host      = tokens[0];
                m_arguments = tokens[1];
            }
            return; // ----->
        } else
            address_port = lowerCase(tokens[1]);
    } else
        address_port = tokens[0];
    
    tokens = assign(split(address_port, PORT_DELIMITER));

    if (tokens.size() < 1 || tokens.size() > 2)
        throw std::runtime_error("uri '" + m_source +
            "' parsing error: too many port delimeters '" + PORT_DELIMITER + "' found"); // ----->

    string address = tokens[0];

    {
        auto address_path_tokens = split(address, "/", 2);
        if  (address_path_tokens.size() == 2) {
            address = address_path_tokens.front();
            m_path = "/" + address_path_tokens.back();
        }
    }

    if (tokens.size() == 2)
        m_port = convert<TPort>(tokens[1]);
    else {
        if (m_protocol > 0)
            m_port = static_cast<TPort>(m_protocol);
    }

    m_host = address;
    
    try {
        m_ipv4 = std::make_shared<TIPv4>(convert<TIPv4>(address));
    } catch (...) {}

    // port from protocol default port
    if (m_port > 0 && m_host.empty() && !m_ipv4)
        throw std::runtime_error("uri '" + m_source + "' parsing error: host or ip not found"); // ----->
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


URI::TPort URI::getPort() const {
    return m_port; // ----->
}


std::string URI::getHost() const {
    return m_host; // ----->
}


std::string URI::getPath() const {
    return m_path; // ----->
}


URI::TProtocol URI::getProtocol() const {
    return m_protocol; // ----->
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
