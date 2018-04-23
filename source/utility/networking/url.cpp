#include "url.h"

#include "utility/strings.h"
#include "utility/items.h"
#include "dns.h"

#include "utility/logging/logger.h"


IMPLEMENT_ENUM(utility::networking::URL::TProtocol)


using std::make_shared;
using std::string;
using std::vector;
using utility::convertion::convert;
using utility::convertion::convertPtr;


//template<>
//size_t std::hash<utility::networking::URL>() {

//}


namespace {


string const PROTOCOL_DELIMITER = "://";
string const PORT_DELIMITER     = ":";
//string const IPv4_REGEX         = "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$";


} // unnamed


namespace utility {
namespace networking {


URL::URL(std::string const &url)
:
    m_address(lowerCase(url))
{
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

    tokens = assign(split(address_port, PORT_DELIMITER));

    if (tokens.size() < 1 || tokens.size() > 2)
        throw std::runtime_error("wrong url '" + m_address + "'"); // ----->

    string address = tokens[0];

    if (tokens.size() == 2)
        m_port = make_shared<TPort>(convert<TPort>(tokens[1]));
    else {
        if (m_protocol && *m_protocol != TProtocol::UNKNOWN && *m_protocol > 0) {
            m_port = make_shared<TPort>(static_cast<TPort>(*m_protocol));
        }
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
            return make_shared<TIPv4 const>(getIPv4ByHost(*m_host)); // ----->
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


URL::TProtocol const URL::getProtocol() const {
    if (m_protocol)
        return *m_protocol; // ----->
    else
        return TProtocol::UNKNOWN;
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


} // networking
} // utility


namespace {


utility::networking::URL createURL(string const &value) {
    return utility::networking::URL(value); // ----->
}


string getAddress(utility::networking::URL const &url) {
    return url.getAddress(); // ----->
}


} // unnamed


IMPLEMENT_CONVERT(utility::networking::URL, std::string, createURL)
IMPLEMENT_CONVERT(std::string, utility::networking::URL, getAddress)
