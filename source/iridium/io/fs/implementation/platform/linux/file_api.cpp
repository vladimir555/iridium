#include "file_api.h"


#if defined(LINUX_PLATFORM) || defined(EMSCRIPTEN_PLATFORM)


#include "../../file_api_assert.h"

#include <cstring>
#include <sys/stat.h>
#include <sys/file.h>

#include "iridium/items.h"


namespace iridium {
namespace io {
namespace fs {
namespace implementation {
namespace platform {


int getFD(::FILE *file) {
    return ::fileno(file); // ----->
}


iridium::io::fs::TFileStatus getFileStatus(::FILE *file) {
    struct stat64 file_stat = {};

    auto result = fstat64(getFD(file), &file_stat);
    assertOK(result, ::strerror(errno));

    std::chrono::system_clock::time_point tp {
        std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::seconds       {file_stat.st_mtim.tv_sec} +
            std::chrono::nanoseconds   {file_stat.st_mtim.tv_nsec}
        )
    };

    return iridium::io::fs::TFileStatus {
        tp,
        static_cast<size_t>(file_stat.st_size)
    }; // ----->
}


::FILE *open(std::string const &file_name, std::string const &open_mode) {
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


} // platform
} // implementation
} // fs
} // io
} // iridium


#endif // LINUX_PLATFORM
