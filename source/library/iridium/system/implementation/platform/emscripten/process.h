#ifndef HEADER_PROCESS_38290575_4B13_468F_8800_55489FFD5F29
#define HEADER_PROCESS_38290575_4B13_468F_8800_55489FFD5F29


#include "iridium/platform.h"


#ifdef EMSCRIPTEN_PLATFORM


#include "../unix/process.h"


namespace iridium::system::implementation::platform {


typedef platform::unix_::CProcessStream CProcessStream;


} // iridium::system::implementation::platform


#endif // EMSCRIPTEN_PLATFORM


#endif // HEADER_PROCESS_38290575_4B13_468F_8800_55489FFD5F29
