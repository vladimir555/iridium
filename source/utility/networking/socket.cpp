#include "socket.h"

#include <netdb.h>

#include "utility/assert.h"


typedef utility::networking::ISocketStream::TPacket TPacket;
using std::string;


namespace {


TPacket convertStringToPacket(string const &source) {
    return TPacket(source.begin(), source.end()); // ----->
}


string convertPacketToString(TPacket const &source) {
    return string(source.begin(), source.end()); // ----->
}


} // unnamed


IMPLEMENT_CONVERT(TPacket, string, convertStringToPacket)
IMPLEMENT_CONVERT(string, TPacket, convertPacketToString)
