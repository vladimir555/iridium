/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <iridium/testing/tester.h>
#include <iridium/convertion/convert.h>
//#include <iridium/caching/lazy_cacher.h>
#include <iridium/platform.h>
#include <iridium/enum.h>


#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <chrono>
#include <functional>


using namespace std;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::system_clock;


enum TAnotherEnum {
    AE1 = 55,
    AE2 = 555
};


DEFINE_ENUM(TEnum, E1, E2, E3 = 5, E4 = AE1, E5)
IMPLEMENT_ENUM(TEnum)


namespace iridium {
namespace convertion {


//template<typename TValue>
//TValue makeTestValue(int const &i);


//template<>
//int makeTestValue(int const &i) {
//    return i; // ----->
//}


//template<>
//double makeTestValue(int const &i) {
//    return i / 5; // ----->
//}


//template<>
//string makeTestValue(int const &i) {
//    return convert<string>(i); // ----->
//}


//template<typename TFrom, typename TTo>
//void runBenchmark() {
//    cout << "convert"                                   << endl
//         << "from: " << string(typeid(TFrom).name())    << endl
//         << "to  : " << string(typeid(TTo).name())      << endl;


//#ifdef MACOS_PLATFORM
//    static auto const item_count        = 5000;
//    static auto const iteration_count   = 5;
//# endif
//#ifdef LINUX_PLATFORM
//    static auto const item_count        = 5000;
//    static auto const iteration_count   = 5;
//# endif
//#ifdef FREEBSD_PLATFORM
//    static auto const item_count        = 5000;
//    static auto const iteration_count   = 5;
//# endif
//#ifdef WINDOWS_PLATFORM
//    static auto const item_count        = 100000;
//    static auto const iteration_count   = 10;
//# endif


//    vector<TFrom> from;

//    for (int i = 0; i < item_count; i++) {
//        TFrom const f = makeTestValue<TFrom>(i);
//        from.push_back(f);
//    }

//    int64_t time_convert = 0;
//    {
//        vector<TTo> to(item_count);
//        auto begin      = high_resolution_clock::now();

//        for (size_t n = 0; n < iteration_count; n++)
//            for (size_t i = 0; i < item_count; i++)
//                to[i] = convert<TTo>(static_cast<TFrom>(from[i]));

//        auto end        = high_resolution_clock::now();
//        time_convert    = duration_cast<nanoseconds>(end - begin).count();
//    }

//    int64_t time_cached_convert = 0;
//    {
//        vector<TTo> to(item_count);
//        auto begin      = high_resolution_clock::now();

//        auto convertCached = std::bind(
//                    iridium::caching::LazyCacher<TTo, TFrom>::cacheFunc,
//                    convert<TTo, TFrom>,
//                    std::placeholders::_1);

//        for (size_t n = 0; n < iteration_count; n++)
//            for (size_t i = 0; i < item_count; i++)
//                to[i] = convertCached(static_cast<TFrom>(from[i]));

//        auto end              = high_resolution_clock::now();
//        time_cached_convert   = duration_cast<nanoseconds>(end - begin).count();
//    }

//    cout << "convert        time: " << time_convert << " ns" << endl;
//    cout << "convert cached time: " << time_cached_convert << " ns" << endl;
//    cout << "convert vs cached convert: " << (time_convert * 100 / time_cached_convert) << " %" << endl;
//    cout << endl;
//}


//TEST(convertion, benchmark) {
//    runBenchmark<int    , string>();
//    runBenchmark<double , string>();
//    runBenchmark<string , int>();
//    runBenchmark<string , double>();
//}


TEST(types) {
    ASSERT("true"   , equal, convert<string>(true));
    ASSERT("false"  , equal, convert<string>(false));

    ASSERT("12345"  , equal, convert<string>(static_cast<uint64_t>(12345)));
    ASSERT("5"      , equal, convert<string>(static_cast<uint64_t>(5)));
    ASSERT("5"      , equal, convert<string>(static_cast<uint32_t>(5)));
    ASSERT("-5"     , equal, convert<string>(static_cast<int64_t>(-5)));
    ASSERT("-5"     , equal, convert<string>(static_cast<int32_t>(-5)));
    ASSERT("0"      , equal, convert<string>(static_cast<int64_t>(0)));
    ASSERT("0"      , equal, convert<string>(static_cast<int32_t>(0)));
    ASSERT("F"      , equal, convert<string>(15, 16));

    ASSERT("5.50000", equal, convert<string>(5.5));
    ASSERT("5.56"   , equal, convert<string>(5.556, 2));
    ASSERT("5.55"   , equal, convert<string>(5.554, 2));
    ASSERT("625.10" , equal, convert<string>(625.1, 2));
    ASSERT("-5.50000",equal, convert<string>(-5.5));
    ASSERT("-5.56"  , equal, convert<string>(-5.556, 2));
    ASSERT("-5.55"  , equal, convert<string>(-5.554, 2));
    ASSERT( "17976931348623157", equal, convert<string>( std::numeric_limits<double>::max()).substr(0, 17));
    ASSERT("-17976931348623157", equal, convert<string>(-std::numeric_limits<double>::max()).substr(0, 18));

    ASSERT(true     , equal, convert<bool>(string("True")));
    ASSERT(false    , equal, convert<bool>(string("false")));

    ASSERT(convert<bool>(string("wrong true")), std::exception);
    ASSERT(convert<bool>(string(""))          , std::exception);

    ASSERT(0        , equal, convert<int32_t>(string("0")));
    ASSERT(5        , equal, convert<int32_t>(string("5")));
    ASSERT(-5       , equal, convert<int32_t>(string("-5")));

    ASSERT(convert<int32_t>(string("5 wrong")), std::exception);
    ASSERT(convert<int32_t>(string(""))       , std::exception);

    ASSERT(5.5      , equal, convert<double>(string("5.5")));
    ASSERT(-5.5     , equal, convert<double>(string("-5.5")));

    ASSERT(static_cast<double>(525.1), equal, convert<double>(string("525.1")));

    ASSERT(convert<double>(string("5.5 wrong")) , std::exception);
    ASSERT(convert<double>(string(""))          , std::exception);

    typedef system_clock::time_point TTime;
    using std::chrono::seconds;
    auto to_time_t = [] (TTime const &t) {
        return time_t(std::chrono::duration_cast<std::chrono::seconds>(t.time_since_epoch()).count());
    };

    ASSERT("2015-05-05 05:05:05.000", equal, convert<string>(TTime(seconds(1430802305))));

    ASSERT(to_time_t(convert<TTime>(string("2015-05-05 05:05:05.000"))), equal, 1430802305);
    ASSERT(TTime(std::chrono::milliseconds(555)), equal, convert<TTime>(string("1970-01-01 00:00:00.555")));

#ifdef PLATFORM_CPU64
    ASSERT(4449517261, equal, to_time_t(convert<TTime>(string("2111-01-01 01:01:01.000"))));
#endif

    ASSERT(convert<TTime>(string("2015-05-05 05:05:05.000 wrong")), std::exception);

#ifndef _WIN32
    ASSERT(string(u8"ħëłlö"), equal, convert<string>(wstring(L"ħëłlö")));
#endif // !_WIN32
    //uint8_t  const hello_s[] = { 0xC4, 0xA7, 0xC3, 0xAB, 0xC5, 0x82, 0x6C, 0xC3, 0xB6 };
    //uint32_t const hello_w[] = { 0xC4, 0xA7, 0xC3, 0xAB, 0xC5, 0x82, 0x6C, 0xC3, 0xB6 };
    //wstring hello_wstr(hello_w, hello_w + sizeof(hello_w[0]) * 9);
    //string  hello_str (hello_s, hello_s + sizeof(hello_s[0]) * 9);
    //LOGT << hello_str;
    //LOGT << convert<string>(hello_wstr);
    //ASSERT(hello_str, equal, convert<string>(hello_wstr));
}


enum TEnumTest {
    E1, E2, E3 = 5, E4 = AE1, E5
};


TEST(enum_) {
    ASSERT(TEnum::E1, equal, static_cast<int>(TEnumTest::E1));
    ASSERT(TEnum::E2, equal, static_cast<int>(TEnumTest::E2));
    ASSERT(TEnum::E3, equal, static_cast<int>(TEnumTest::E3));
    ASSERT(TEnum::E4, equal, static_cast<int>(TEnumTest::E4));
    ASSERT(TEnum::E5, equal, static_cast<int>(TEnumTest::E5));

    ASSERT(static_cast<int>(TEnumTest::E1), equal, convert<TEnum>(string("E1")));
    ASSERT(static_cast<int>(TEnumTest::E2), equal, convert<TEnum>(string("E2")));
    ASSERT(static_cast<int>(TEnumTest::E3), equal, convert<TEnum>(string("E3")));
    ASSERT(static_cast<int>(TEnumTest::E4), equal, convert<TEnum>(string("E4")));
    ASSERT(static_cast<int>(TEnumTest::E5), equal, convert<TEnum>(string("E5")));

    ASSERT("E1", equal, convert<string>(TEnum::E1));
    ASSERT("E2", equal, convert<string>(TEnum::E2));
    ASSERT("E3", equal, convert<string>(TEnum::E3));
    ASSERT("E4", equal, convert<string>(TEnum::E4));
    ASSERT("E5", equal, convert<string>(TEnum::E5));

    {
        TEnum e = TEnum::E1;
        ASSERT("E1", equal, convert<string>(e));
    }

    {
        TEnum e = static_cast<TEnum::TEnumInternal>(1);
        ASSERT("E2", equal, convert<string>(e));
    }

    ASSERT(convert<TEnum>(string("E55")), std::runtime_error);
    ASSERT("UNKNOWN", equal, convert<string>(TEnum(static_cast<TEnum::TEnumInternal>(10))));

    std::list<TEnum::TEnumInternal> l;
    for (auto const &i: TEnum::getEnums())
        l.push_back(i);

    ASSERT(std::list<TEnum::TEnumInternal>( { TEnum::E1, TEnum::E2, TEnum::E3, TEnum::E4, TEnum::E5 } ), equal, l);
}


} // convertion
} // iridium
