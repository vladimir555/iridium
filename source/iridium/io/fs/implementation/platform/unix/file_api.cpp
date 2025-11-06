#include "file_api.h"


#ifdef UNIX_PLATFORM


#include "iridium/items.h"
#include "iridium/io/fs/implementation/file_api_assert.h"

#include <cstring>
#include <sys/stat.h>
#include <sys/file.h>


namespace iridium {
namespace io {
namespace fs {
namespace implementation {
namespace platform {
namespace unix_ {


::FILE *open(const std::string &file_name, const std::string &open_mode) {
    ::FILE *file = nullptr;
    file = assertOK(::fopen(file_name.c_str(), open_mode.c_str()), ::strerror(errno));
    if (checkOneOf(open_mode.substr(0, 1), "a", "w"))
        assertOK(flock(getFD(file), LOCK_EX | LOCK_NB), ::strerror(errno));
    return file; // ----->
}


void close(::FILE *file) {
    if (file) {
        flock(getFD(file), LOCK_UN);
        assertOK(::fclose(file), ::strerror(errno));
    }
}


} // unix_
} // platform
} // implementation
} // fs
} // io
} // iridium


#endif // UNIX_PLATFORM
