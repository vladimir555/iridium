/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
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

#include <gtest/gtest.h>


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
//#endif
//#ifdef LINUX_PLATFORM
//    static auto const item_count        = 5000;
//    static auto const iteration_count   = 5;
//#endif
//#ifdef FREEBSD_PLATFORM
//    static auto const item_count        = 5000;
//    static auto const iteration_count   = 5;
//#endif
//#ifdef WINDOWS_PLATFORM
//    static auto const item_count        = 100000;
//    static auto const iteration_count   = 10;
//#endif


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
    ASSERT_EQ( "17976931348623157", convert<string>( DBL_MAX).substr(0, 17));
    ASSERT_EQ("-17976931348623157", convert<string>(-DBL_MAX).substr(0, 18));

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

    typedef system_clock::time_point TTime;
    using std::chrono::seconds;
    auto to_time_t = [] (TTime const &t) {
        return time_t(std::chrono::duration_cast<std::chrono::seconds>(t.time_since_epoch()).count());
    };

    ASSERT_EQ("2015-05-05 05:05:05.000", convert<string>(TTime(seconds(1430802305))));

    ASSERT_EQ(1430802305, to_time_t(convert<TTime>(string("2015-05-05 05:05:05.000"))));
    ASSERT_EQ(TTime(std::chrono::milliseconds(555)), convert<TTime>(string("1970-01-01 00:00:00.555")));

#ifdef PLATFORM_CPU64
    ASSERT_EQ(4449517261, to_time_t(convert<TTime>(string("2111-01-01 01:01:01.000"))));
#endif

    ASSERT_THROW(convert<TTime>(string("2015-05-05 05:05:05.000 wrong")), std::exception);

    ASSERT_EQ("ħëłlö", convert<string>(wstring(L"ħëłlö")));
}


enum TEnumTest {
    E1, E2, E3 = 5, E4 = AE1, E5
};


TEST(convertion, enum_) {
    ASSERT_EQ(static_cast<int>(TEnumTest::E1), TEnum::E1);
    ASSERT_EQ(static_cast<int>(TEnumTest::E2), TEnum::E2);
    ASSERT_EQ(static_cast<int>(TEnumTest::E3), TEnum::E3);
    ASSERT_EQ(static_cast<int>(TEnumTest::E4), TEnum::E4);
    ASSERT_EQ(static_cast<int>(TEnumTest::E5), TEnum::E5);

    ASSERT_EQ(static_cast<int>(TEnumTest::E1), convert<TEnum>(string("E1")));
    ASSERT_EQ(static_cast<int>(TEnumTest::E2), convert<TEnum>(string("E2")));
    ASSERT_EQ(static_cast<int>(TEnumTest::E3), convert<TEnum>(string("E3")));
    ASSERT_EQ(static_cast<int>(TEnumTest::E4), convert<TEnum>(string("E4")));
    ASSERT_EQ(static_cast<int>(TEnumTest::E5), convert<TEnum>(string("E5")));

    ASSERT_EQ("E1", convert<string>(TEnum::E1));
    ASSERT_EQ("E2", convert<string>(TEnum::E2));
    ASSERT_EQ("E3", convert<string>(TEnum::E3));
    ASSERT_EQ("E4", convert<string>(TEnum::E4));
    ASSERT_EQ("E5", convert<string>(TEnum::E5));

    {
        TEnum e = TEnum::E1;
        convert<string>(e);
    }

    {
        TEnum e = static_cast<TEnum::TEnumInternal>(1);
        convert<string>(e);
    }

    ASSERT_THROW(convert<TEnum>(string("E55")), std::runtime_error);
    ASSERT_THROW(convert<string>(TEnum(static_cast<TEnum::TEnumInternal>(10))), std::runtime_error);

    std::list<TEnum::TEnumInternal> l;
    for (auto const &i: TEnum::getEnums())
        l.push_back(i);

    ASSERT_EQ(std::list<TEnum::TEnumInternal>( { TEnum::E1, TEnum::E2, TEnum::E3, TEnum::E4, TEnum::E5 } ), l);
}


} // convertion
} // iridium
