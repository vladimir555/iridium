#include "file_api.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/items.h"
#include "iridium/io/fs/implementation/file_api_assert.h"

#include <cstring>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>


namespace iridium::io::fs::implementation::platform {


std::string strerrorInternal(int const &code) {
    char buffer[256];
    strerror_s(buffer, code);
    return buffer; // ----->
}


int getFD(::FILE *file) {
    return ::_fileno(file); // ----->
}


iridium::io::fs::TFileStatus getFileStatus(::FILE *file) {
    struct stat   file_stat = {};

    auto result = fstat(getFD(file), &file_stat);
    assertOK(result, strerrorInternal(errno));

    auto tp = std::chrono::system_clock::from_time_t( file_stat.st_mtime );

    return iridium::io::fs::TFileStatus {
        tp,
        static_cast<size_t>(file_stat.st_size)
    }; // ----->
}


::FILE *open(std::string const &file_name, std::string const &open_mode) {
    ::FILE *file = nullptr;
    file = assertOK(::_fsopen(file_name.c_str(), open_mode.c_str(), _SH_DENYWR), strerrorInternal(errno));
    return file; // ----->
}


void close(::FILE *file) {
    if (file)
        assertOK(::fclose(file), strerrorInternal(errno));
}



} // iridium::io::fs::implementation::platform


#endif // WINDOWS_PLATFORM
