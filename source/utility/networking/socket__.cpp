#include "socket.h"


typedef utility::networking::ISocketStream::packet_t packet_t;
using std::string;


namespace {


packet_t convertStringToPacket(string const &source) {
    return packet_t(source.begin(), source.end()); // ----->
}


string convertPacketToString(packet_t const &source) {
    return string(source.begin(), source.end()); // ----->
}


} // unnamed


IMPLEMENT_CONVERT(packet_t, string, convertStringToPacket)
IMPLEMENT_CONVERT(string, packet_t, convertPacketToString)
