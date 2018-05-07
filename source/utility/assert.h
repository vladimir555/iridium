#ifndef HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED
#define HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED


#include <stdexcept>
#include <string>

#include "utility/convertion/convert.h"


namespace utility {


template<typename T>
T &assertExists(T &&t, std::string const &error) {
    if (t)
        return t; // ----->
    else
        throw std::runtime_error(error);
}


template<typename T>
T &assertSize(T &&t, size_t const &size, std::string const &error) {
    if (t.size() == size)
        return t; // ----->
    else
        throw std::runtime_error(error + ", wrong items size " +
            convertion::convert<std::string>(t.size()) + ", expects " +
            convertion::convert<std::string>(size));
}


template<typename T>
T &assertOne(T &&t, std::string const &error) {
    return assertSize(t, 1, error); // ----->
}


template<typename T>
T &assertComplete(T &&t, std::string const &error) {
    if (t.size() >= 1)
        return t; // ----->
    else
        throw std::runtime_error(error);
}


} // utility


#endif // HEADER_ASSERT_FC81EE1B_9E8E_41B2_9F3E_B3F87F1DA4ED
