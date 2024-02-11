// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED
#define HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED


#include <stdexcept>
#include <string>

#include "iridium/convertion/convert.h"


namespace iridium {


template<typename T>
T assertExists(T &&value, std::string const &error) {
    if (static_cast<bool>(value))
        return std::forward<T>(value); // ----->
    else
        throw std::runtime_error(error);
}


template<typename T>
T *assertExists(T *value, std::string const &error) {
    if (static_cast<bool>(value))
        return value; // ----->
    else
        throw std::runtime_error(error);
}


template<typename T>
std::shared_ptr<T> assertExists(std::shared_ptr<T> const value, std::string const &error) {
    if (static_cast<bool>(value))
        return value; // ----->
    else
        throw std::runtime_error(error);
}


template<typename T>
T assertSize(T &&values, size_t const &size, std::string const &error) {
    if (values.size() == size)
        return std::forward<T>(values); // ----->
    else
        throw std::runtime_error(error + ", wrong items size " +
            convertion::convert<std::string>(values.size()) + ", expects " +
            convertion::convert<std::string>(size));
}


template<typename T>
T assertOne(T &&values, std::string const &error) {
    return std::forward<T>(assertSize(std::move(values), 1, error)); // ----->
}


template<typename T>
T assertComplete(T &&values, std::string const &error) {
    if (values.size() > 0)
        return std::forward<T>(values); // ----->
    else
        throw std::runtime_error(error);
}


} // iridium


#endif // HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED
