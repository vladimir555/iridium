/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "url.h"

#include "iridium/strings.h"
#include "iridium/items.h"
#include "net/dns.h"


using std::make_shared;
using std::string;
using std::vector;
using iridium::convertion::convert;
using iridium::convertion::convertPtr;
using iridium::io::net::TAddress;
using iridium::io::net::TPort;
using iridium::io::net::TIPv4;


IMPLEMENT_ENUM(iridium::io::URL::TProtocol)


namespace iridium {
namespace io {


URL::URL(std::string const &url)
:
    m_address(lowerCase(url))
{
    //string const IPv4_REGEX                = "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$";
    static string const PROTOCOL_DELIMITER = "://";
    static string const PORT_DELIMITER     = ":";

    if (m_address.empty())
        throw std::runtime_error("wrong url '" + m_address + "'"); // ----->

    auto tokens = assign(split(m_address, PROTOCOL_DELIMITER));

    if (tokens.size() > 2)
        throw std::runtime_error("wrong url '" + m_address + "'"); // ----->

    string address_port;
    if (tokens.size() == 2) {
        m_protocol   = make_shared<TProtocol>(convert<TProtocol>(tokens[0]));
        address_port = tokens[1];
    } else
        address_port = tokens[0];

    if (m_protocol && *m_protocol == TProtocol::FILE)
        return;

    tokens = assign(split(address_port, PORT_DELIMITER));

    if (tokens.size() < 1 || tokens.size() > 2)
        throw std::runtime_error("wrong url '" + m_address + "'"); // ----->

    string address = tokens[0];

    {
        auto tokens_ = split(address, "/", 2);
        address = tokens_.front();
        if (tokens_.size() == 2)
            m_path = make_shared<string>("/" + tokens_.back());
    }

    if (tokens.size() == 2)
        m_port = make_shared<TPort>(convert<TPort>(tokens[1]));
    else {
        if (m_protocol && *m_protocol != TProtocol::UNKNOWN && *m_protocol > 0)
            m_port = make_shared<TPort>(static_cast<TPort>(*m_protocol));
    }

    auto ip = split(address, ".");

    m_host = make_shared<string>(address);
    if (ip.size() == 4) {
        try {
            TIPv4 ipv4;
            for (auto const &ip_byte : ip)
                ipv4.push_back(convert<uint8_t>(ip_byte));

            m_ipv4 = make_shared<TIPv4 const>(ipv4);
        } catch (...) {
            m_ipv4.reset();
        }
    }

    // port from protocol default port
    if (!((m_host || m_ipv4) && m_port && *m_port != 0))
        throw std::runtime_error("wrong url '" + m_address + "'"); // ----->
}


URL::TIPv4SharedPtr const URL::getIPv4() const {
    if (m_ipv4)
        return m_ipv4; // ----->
    else {
        if (m_host)
            return make_shared<TIPv4 const>(net::getIPv4ByHost(*m_host)); // ----->
        else
            return nullptr; // ----->
    }
}


std::string const URL::getIPv4AsString() const {
    string result;

    auto ipv4 = getIPv4();
    if (ipv4 && ipv4->size() == 4)
        for (auto const ip : (*ipv4))
            result += convert<string>(ip) + ".";
    else
        return ""; // ----->

    return result.substr(0, result.size() - 1); // ----->
}


URL::TPortSharedPtr const URL::getPort() const {
    return m_port; // ----->
}


URL::THostSharedPtr const URL::getHost() const {
    return m_host; // ----->
}


URL::TPathSharedPtr const URL::getPath() const {
    return m_path; // ----->
}


URL::TProtocol const URL::getProtocol() const {
    if (m_protocol)
        return *m_protocol; // ----->
    else
        return TProtocol::UNKNOWN; // ----->
}


TAddress const URL::getAddress() const {
    return m_address; // ----->
}


bool URL::operator == (URL const &url) const {
    return (( m_host ==  url.m_host) &&
            ((m_port &&  url.m_port  && (*m_port == *url.m_port)) ||
            (!m_port || !url.m_port))); // ----->
}


bool URL::operator <  (URL const &url) const {
    return (m_host <  url.m_host) ||
          ((m_host == url.m_host) && m_port && url.m_port && (*m_port < *url.m_port)); // ----->
}


} // io
} // iridium


namespace {


iridium::io::URL createURL(string const &value) {
    return iridium::io::URL(value); // ----->
}


string getAddress(iridium::io::URL const &url) {
    return url.getAddress(); // ----->
}


} // unnamed


IMPLEMENT_CONVERT(iridium::io::URL, std::string, createURL)
IMPLEMENT_CONVERT(std::string, iridium::io::URL, getAddress)
