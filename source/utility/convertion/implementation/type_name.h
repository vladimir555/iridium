#ifndef HEADER_TYPE_NAME_661E1CCF_1F7C_495A_A751_095975AE071E
#define HEADER_TYPE_NAME_661E1CCF_1F7C_495A_A751_095975AE071E


#include <string>


namespace utility {
namespace convertion {
namespace implementation {


template<typename T>
std::string getTypeName();


} // implementation
} // convertion
} // utility


#define IMPLEMENT_TYPE_NAME(type) \
namespace utility { \
namespace convertion { \
namespace implementation { \
template<> \
std::string getTypeName<type>() { \
    return #type; \
}}}}


#endif // HEADER_TYPE_NAME_661E1CCF_1F7C_495A_A751_095975AE071E
