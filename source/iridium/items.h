// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ITEMS_F019CA04_62F0_4525_996A_15F23B36F7D5
#define HEADER_ITEMS_F019CA04_62F0_4525_996A_15F23B36F7D5


#include <algorithm>
#include <list>
#include <vector>


namespace iridium {


template<typename T>
void addUnique(typename T::value_type const &i, T &t) {
    if (std::find(t.begin(), t.end(), i) == t.end())
        t.push_back(i);
}


template<typename T>
std::vector<T> assign(std::list<T> const &source) {
    return std::vector<T> {
        std::make_move_iterator(std::begin(source)),
        std::make_move_iterator(std::end(source))
    }; // ----->
}


} // iridium


#endif // HEADER_ITEMS_F019CA04_62F0_4525_996A_15F23B36F7D5
