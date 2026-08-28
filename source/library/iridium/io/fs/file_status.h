#ifndef HEADER_FILE_STATUS_DB277AFC_11F0_4FB7_847B_6EB05C1C786F
#define HEADER_FILE_STATUS_DB277AFC_11F0_4FB7_847B_6EB05C1C786F


// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

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
