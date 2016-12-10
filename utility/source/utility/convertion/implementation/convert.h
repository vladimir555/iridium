#ifndef HEADER_CONVERT_B235DF7F_6B26_4C85_A676_53C18B086095
#define HEADER_CONVERT_B235DF7F_6B26_4C85_A676_53C18B086095


#include <string>
#include <chrono>
#include <thread>


namespace utility {
namespace convertion {
namespace implementation {


// ----- interface


template<typename TResult, typename TValue>
TResult convert(TValue const &value);


template<typename TResult, typename TValue, typename TFormat>
TResult convert(TValue const &value, TFormat const &format);


// ----- implementation


template<>
std::string convert(bool const &value);


template<>
std::string convert(int32_t const &value, int const &base);


template<>
std::string convert(int32_t const &value);


template<>
std::string convert(uint32_t const &value, int const &base);


template<>
std::string convert(uint32_t const &value);


template<>
std::string convert(int64_t const &value, int const &base);


template<>
std::string convert(int64_t const &value);


template<>
std::string convert(uint64_t const &value, int const &base);


template<>
std::string convert(uint64_t const &value);


template<>
std::string convert(uint16_t const &value, int const &base);


template<>
std::string convert(uint16_t const &value);


template<>
std::string convert(uint8_t const &value, int const &base);


template<>
std::string convert(uint8_t const &value);


template<>
std::string convert(double const &value, int const &base);


template<>
std::string convert(double const &value);


template<>
std::string convert(std::chrono::system_clock::time_point const &value);


template<>
std::string convert(std::string const &value);


template<>
std::string convert(std::wstring const &value);


template<>
std::string convert(std::thread::id const &value);


template<>
bool convert(std::string const &value);


template<>
int32_t convert(std::string const &value);


template<>
uint32_t convert(std::string const &value);


template<>
int64_t convert(std::string const &value);


template<>
uint64_t convert(std::string const &value);


template<>
uint16_t convert(std::string const &value);


template<>
uint8_t convert(std::string const &value);


template<>
double convert(std::string const &value);


template<>
std::chrono::system_clock::time_point convert(std::string const &value);


template<>
std::wstring convert(std::string const &value);


} // implementation
} // convertion
} // utility


#endif // HEADER_CONVERT_B235DF7F_6B26_4C85_A676_53C18B086095
