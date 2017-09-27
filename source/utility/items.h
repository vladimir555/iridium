#ifndef HEADER_ITEMS_F019CA04_62F0_4525_996A_15F23B36F7D5
#define HEADER_ITEMS_F019CA04_62F0_4525_996A_15F23B36F7D5


#include <algorithm>


namespace utility {


template<typename T>
bool isLastItem(typename T::value_type const &i, T const &t) {
    return i == *(--t.end());
}


template<typename T>
void addUnique(typename T::value_type const &i, T &t) {
    if (std::find(t.begin(), t.end(), i) == t.end())
        t.push_back(i);
}


} // utility


#endif // HEADER_ITEMS_F019CA04_62F0_4525_996A_15F23B36F7D5
