#ifndef HEADER_FILE_STATUS_DB277AFC_11F0_4FB7_847B_6EB05C1C786F
#define HEADER_FILE_STATUS_DB277AFC_11F0_4FB7_847B_6EB05C1C786F


#include <chrono>


namespace iridium::io::fs {


struct TFileStatus {
    std::chrono::system_clock::time_point
        last_modified;
    size_t
        size;
};


} // iridium::io::fs


#endif // HEADER_FILE_STATUS_DB277AFC_11F0_4FB7_847B_6EB05C1C786F
