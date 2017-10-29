#include "socket.h"

#include <netdb.h>

#include "utility/assert.h"


typedef utility::networking::ISocketStream::TPacket TPacket;
using std::string;


//utility::threading::implementation::CMutex utility::networking::ISocketStream::map_url_read_cache_mutex;
//auto utility::networking::ISocketStream::map_url_read_cache         = std::map<std::string, TPacket>();


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
