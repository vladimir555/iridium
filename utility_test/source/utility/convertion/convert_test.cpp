#include <utility/convertion/convert.h>
#include <utility/platform.h>

#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <chrono>

#include <gtest/gtest.h>


using namespace std;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;


namespace utility {
namespace convertion {


template<typename TValue>
TValue makeTestValue(int const &i);


template<>
int makeTestValue(int const &i) {
    return i; // ----->
}


template<>
double makeTestValue(int const &i) {
    return i / 5; // ----->
}


template<>
string makeTestValue(int const &i) {
    return convert<string>(i); // ----->
}


template<typename TFrom, typename TTo>
void runBenchmark() {
    cout << "convert"                                   << endl
         << "from: " << string(typeid(TFrom).name())    << endl
         << "to  : " << string(typeid(TTo).name())      << endl;


#ifdef MACOS_PLATFORM
    static auto const item_count        = 5000;
    static auto const iteration_count   = 5;
#elif LINUX_PLATFORM
    static auto const item_count        = 5000;
    static auto const iteration_count   = 5;
#elif  FREEBSD_PLATFORM
    static auto const item_count        = 5000;
    static auto const iteration_count   = 5;
#elif  WINDOWS_PLATFORM
    static auto const item_count        = 100000;
    static auto const iteration_count   = 10;
#endif


    vector<TFrom> from;

    for (int i = 0; i < item_count; i++) {
        TFrom const f = makeTestValue<TFrom>(i);
        from.push_back(f);
    }

    int64_t time_convert = 0;
    {
        vector<TTo> to(item_count);
        auto begin      = system_clock::now();

        for (size_t n = 0; n < iteration_count; n++)
            for (size_t i = 0; i < item_count; i++)
                to[i] = convert<TTo>(static_cast<TFrom>(from[i]));

        auto end        = system_clock::now();
        time_convert = duration_cast<nanoseconds>(end - begin).count();
    }

//    int64_t time_cached_convert = 0;
//    {
//        vector<TTo> to(item_count);
//        auto begin      = system_clock::now();

//        auto convertCached = std::bind(
//                    utility::caching::LazyCacher<TTo, TFrom>::cacheFunc,
//                    convert<TTo, TFrom>,
//                    std::placeholders::_1);

//        for (size_t n = 0; n < iteration_count; n++)
//            for (size_t i = 0; i < item_count; i++)
//                to[i] = convertCached<TTo>(static_cast<TFrom>(from[i]));

//        auto end              = system_clock::now();
//        time_cached_convert   = duration_cast<nanoseconds>(end - begin).count();
//    }

    cout << "convert        time: " << time_convert << " ns" << endl;
//    cout << "convert cached time: " << time_convert << " ns" << endl;
//    cout << "convert vs cached convert: " << (time_convert * 100 / time_cached_convert) << " %" << endl;
    cout << endl;
}


TEST(convertion, benchmark) {
    runBenchmark<int    , string>();
    runBenchmark<double , string>();
    runBenchmark<string , int>();
    runBenchmark<string , double>();
}


TEST(convertion, types) {
    ASSERT_EQ("true"        , convert<string>(true));
    ASSERT_EQ("false"       , convert<string>(false));

    ASSERT_EQ("12345"       , convert<string>(static_cast<uint64_t>(12345)));
    ASSERT_EQ("5"           , convert<string>(static_cast<uint64_t>(5)));
    ASSERT_EQ("5"           , convert<string>(static_cast<uint32_t>(5)));
    ASSERT_EQ("-5"          , convert<string>(static_cast<int64_t>(-5)));
    ASSERT_EQ("-5"          , convert<string>(static_cast<int32_t>(-5)));
    ASSERT_EQ("0"           , convert<string>(static_cast<int64_t>(0)));
    ASSERT_EQ("0"           , convert<string>(static_cast<int32_t>(0)));
    ASSERT_EQ("F"           , convert<string>(15, 16));

    ASSERT_EQ("5.50000"     , convert<string>(5.5));
    ASSERT_EQ("5.56"        , convert<string>(5.556, 2));
    ASSERT_EQ("5.55"        , convert<string>(5.554, 2));
    ASSERT_EQ("625.10"      , convert<string>(625.1, 2));
    ASSERT_EQ("-5.50000"    , convert<string>(-5.5));
    ASSERT_EQ("-5.56"       , convert<string>(-5.556, 2));
    ASSERT_EQ("-5.55"       , convert<string>(-5.554, 2));
    ASSERT_EQ("ħëłlö"       , convert<string>(wstring(L"ħëłlö")));

    ASSERT_EQ("2015-05-05 05:05:05.000", convert<string>(system_clock::from_time_t(1430802305)));

    ASSERT_EQ(true          , convert<bool>(string("True")));
    ASSERT_EQ(false         , convert<bool>(string("false")));
    ASSERT_THROW(convert<bool>(string("wrong true"))    , std::exception);
    ASSERT_THROW(convert<bool>(string(""))              , std::exception);

    ASSERT_EQ(0             , convert<int32_t>(string("0")));
    ASSERT_EQ(5             , convert<int32_t>(string("5")));
    ASSERT_EQ(-5            , convert<int32_t>(string("-5")));
    ASSERT_THROW(convert<int32_t>(string("5 wrong"))    , std::exception);
    ASSERT_THROW(convert<int32_t>(string(""))           , std::exception);

    ASSERT_DOUBLE_EQ(5.5    , convert<double>(string("5.5")));
    ASSERT_DOUBLE_EQ(-5.5   , convert<double>(string("-5.5")));
    ASSERT_DOUBLE_EQ(static_cast<double>(525.1) , convert<double>(string("525.1")));
    ASSERT_THROW(convert<double>(string("5.5 wrong"))   , std::exception);
    ASSERT_THROW(convert<double>(string(""))            , std::exception);

    ASSERT_EQ(1430802305,
              system_clock::to_time_t(convert<system_clock::time_point>(
              string("2015-05-05 05:05:05.000"))));
    ASSERT_EQ(1430784000,
              system_clock::to_time_t(convert<system_clock::time_point>(
              string("2015-05-05 00:00:00.000"))));
#ifdef PLATFORM_CPU64
    ASSERT_EQ(4449517261,
              system_clock::to_time_t(convert<system_clock::time_point>(
              string("2111-01-01 01:01:01.000"))));
#endif
    ASSERT_THROW(convert<system_clock::time_point>(string("2015-05-05 05:05:05.000 wrong")), std::exception);
}


} // convertion
} // utility
