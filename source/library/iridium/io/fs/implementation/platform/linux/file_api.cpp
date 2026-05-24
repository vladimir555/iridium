#include "file_api.h"


#if defined(LINUX_PLATFORM) || defined(EMSCRIPTEN_PLATFORM)


#include "iridium/items.h"
#include "iridium/io/fs/implementation/file_api_assert.h"

#include <cstring>
#include <sys/stat.h>
#include <sys/file.h>


namespace iridium::io::fs::implementation::platform {


int getFD(::FILE *file) {
    return ::fileno(file); // ----->
}


iridium::io::fs::TFileStatus getFileStatus(::FILE *file) {
    struct stat file_stat = {};

    auto result = fstat(getFD(file), &file_stat);
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


} // iridium::io::fs::implementation::platform


#endif // LINUX_PLATFORM
