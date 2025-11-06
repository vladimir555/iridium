#include "file_api.h"


#ifdef FREEBSD_LIKE_PLATFORM


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


int getFD(::FILE *file) {
    return file->_file; // ----->
}


iridium::io::fs::TFileStatus getFileStatus(::FILE *file) {
    struct stat file_stat   = {};
    auto        result      = fstat(getFD(file), &file_stat);

    assertOK(result, ::strerror(errno));

    std::chrono::system_clock::time_point tp {
        std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::seconds       {file_stat.st_mtimespec.tv_sec} +
            std::chrono::nanoseconds   {file_stat.st_mtimespec.tv_nsec}
        )
    };

    return iridium::io::fs::TFileStatus {
        tp,
        static_cast<size_t>(file_stat.st_size)
    }; // ----->
}


} // platform
} // implementation
} // fs
} // io
} // iridium


#endif // FREEBSD_LIKE_PLATFORM
