#include "packet.h"

#include "utility/logging/logger.h"

using std::string;
using utility::protocol::IPacket;


namespace utility {
namespace protocol {
namespace implementation {


CPacket::CPacket(TData const &data, bool const &is_last)
:
    m_data      (data),
    m_is_last   (is_last)
{}


IPacket::TData CPacket::getData() const {
    return m_data; // ----->
}


bool CPacket::getIsLast() const {
    return m_is_last; // ----->
}


} // implementation
} // protocol
} // utility


namespace {


IPacket::TData convertStringToData(string const &source) {
    return IPacket::TData(source.begin(), source.end()); // ----->
}


string convertDataToString(IPacket::TData const &source) {
    return string(source.begin(), source.end()); // ----->
}


} // unnamed


IMPLEMENT_CONVERT(utility::protocol::IPacket::TData, std::string, convertStringToData)
IMPLEMENT_CONVERT(std::string, utility::protocol::IPacket::TData, convertDataToString)
