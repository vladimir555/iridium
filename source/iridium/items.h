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


template<typename T, typename ... TItems>
bool checkOneOf(T const &source, TItems const & ... items_) {
    std::initializer_list<T const> const &items{items_ ...};

    for (auto const &item: items)
        if (item == source)
            return true;

    return false; // ----->
}


template<typename TObject, typename ... TArgs>
std::list<typename TObject::TSharedPtr> createObjects(size_t const &count, TArgs ... args) {
    std::list<typename TObject::TSharedPtr> objects;

    for (size_t i = 0; i < count; i++)
        objects.push_back(TObject::create(args ...));

    return objects; // ----->
}


template<typename TInterface, typename TObject, typename ... TArgs>
std::list<typename TInterface::TSharedPtr> createObjects(size_t const &count, TArgs ... args) {
    std::list<typename TInterface::TSharedPtr> objects;

    for (size_t i = 0; i < count; i++)
        objects.push_back(TObject::create(args ...));

    return objects; // ----->
}


} // iridium


#endif // HEADER_ITEMS_F019CA04_62F0_4525_996A_15F23B36F7D5
