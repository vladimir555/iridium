#ifndef HEADER_FILE_STATUS_DB277AFC_11F0_4FB7_847B_6EB05C1C786F
#define HEADER_FILE_STATUS_DB277AFC_11F0_4FB7_847B_6EB05C1C786F


#include <chrono>


namespace iridium {
namespace io {
namespace fs {


/// \~english @brief Represents the status of a file, including its last modification time and size.
/// \~russian @brief Представляет статус файла, включая время последнего изменения и размер.
struct TFileStatus {
    /// \~english @brief The time point of the last modification of the file.
    /// \~russian @brief Момент времени последнего изменения файла.
    std::chrono::system_clock::time_point
        last_modified;
    /// \~english @brief The size of the file in bytes.
    /// \~russian @brief Размер файла в байтах.
    size_t
        size;
};


} // fs
} // io
} // iridium


#endif // HEADER_FILE_STATUS_DB277AFC_11F0_4FB7_847B_6EB05C1C786F
