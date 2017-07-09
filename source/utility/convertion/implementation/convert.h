#ifndef HEADER_CONVERT_7C31FD8D_5F64_4C4D_9D38_BD994242E709
#define HEADER_CONVERT_7C31FD8D_5F64_4C4D_9D38_BD994242E709


#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <atomic>


namespace utility {
namespace convertion {
namespace implementation {


struct config {
    static std::atomic<int> double_precission;
};


// ----- interface


template<typename TResult, typename TValue>
TResult convert(TValue const &value);


template<typename TResult, typename TValue, typename TFormat>
TResult convert(TValue const &value, TFormat const &format);


template<typename TResult, typename TValue>
std::shared_ptr<TResult> convertPtr(TValue const &value) {
    return std::make_shared<TResult>(convert<TResult>(value)); // ----->
}


template<typename TResult>
std::shared_ptr<TResult> convertPtr(std::string const &value) {
    if (value == "NULL")
        return std::shared_ptr<TResult>(nullptr); // ----->

    return std::make_shared<TResult>(convert<TResult>(value)); // ----->
}


template<typename TValue>
std::string convertPtr(std::shared_ptr<TValue> const &value) {
    if (value)
        return convert<std::string>(*value); // ----->
    else
        return "NULL"; // ----->
}


template<typename TValue, typename TFormat>
std::string convertPtr(std::shared_ptr<TValue> const &value, TFormat const &format) {
    if (value)
        return convert<std::string>(*value, format); // ----->
    else
        return "NULL"; // ----->
}


// ----- implementation


template<>
std::string convert(std::chrono::high_resolution_clock::time_point const &value);


template<>
std::string convert(std::chrono::hours const &value);


template<>
std::string convert(std::chrono::minutes const &value);


template<>
std::string convert(std::chrono::seconds const &value);


template<>
std::string convert(std::chrono::milliseconds const &value);


template<>
std::string convert(std::chrono::microseconds const &value);


template<>
std::string convert(std::chrono::nanoseconds const &value);


template<>
std::string convert(bool const &value);


template<>
std::string convert(int64_t const &value, int const &base);


template<>
std::string convert(uint64_t const &value, int const &base);


template<>
std::string convert(int32_t const &value, int const &base);


template<>
std::string convert(uint32_t const &value, int const &base);


template<>
std::string convert(int64_t const &value);


template<>
std::string convert(uint64_t const &value);


template<>
std::string convert(int32_t const &value);


template<>
std::string convert(uint32_t const &value);


template<>
std::string convert(uint16_t const &value);


template<>
std::string convert(uint8_t const &value);


template<>
std::string convert(double const &value, int const &precision);


template<>
std::string convert(double const &value);


template<>
std::string convert(std::thread::id const &value);


template<>
std::chrono::high_resolution_clock::time_point convert(std::string const &value);


template<>
bool convert(std::string const &value_);


template<>
int32_t convert(std::string const &value);


template<>
int64_t convert(std::string const &value);


template<>
uint32_t convert(std::string const &value);


template<>
uint16_t convert(std::string const &value);


template<>
uint8_t convert(std::string const &value);


template<>
uint64_t convert(std::string const &value);


template<>
double convert(std::string const &value);


template<>
std::string convert(std::wstring const &value);


template<>
std::wstring convert(std::string const &value);


} // implementation
} // convertion
} // utility


#endif // HEADER_CONVERT_7C31FD8D_5F64_4C4D_9D38_BD994242E709
