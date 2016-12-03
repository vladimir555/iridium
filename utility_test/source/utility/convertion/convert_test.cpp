#include <utility/convertion/convert.h>
#include <utility/platform.h>

#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>

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
    cout << "convert" << endl
         << "from: " << string(typeid(TFrom).name()) << endl
         << "to:   " << string(typeid(TTo).name()) << endl;


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


} // convertion
} // utility
