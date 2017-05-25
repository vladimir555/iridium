#ifndef HEADER_LAZY_CACHER_472B28A6_2137_4B4F_9C63_BAA8C459D633
#define HEADER_LAZY_CACHER_472B28A6_2137_4B4F_9C63_BAA8C459D633


#include <map>
#include <functional>


namespace utility {
namespace caching {


// ----- interface


template<typename TResult, typename ... TArgs>
class LazyCacher {
public:
    typedef std::function<TResult(TArgs ...)> TFunc;
    static TResult cacheFunc(TFunc func, TArgs const & ... args);
    static void clean();
    static void clean(TArgs const & ... args);
    static void clean(TResult const &result);

private:
    typedef std::tuple<TArgs const & ... >  TArgsTuple;
    typedef std::map<TArgsTuple, TResult>   TArgsTupleResultMap;
    static TArgsTupleResultMap m;
};


// ----- implementation


template<typename TResult, typename ... TArgs>
TResult LazyCacher<TResult, TArgs ... >::cacheFunc(TFunc func, TArgs const & ... args) {
    TArgsTuple args_tuple(args ...);

    auto i = m.find(args_tuple);
    if (i == m.end()) {
        auto r = func(args ...);
        m[args_tuple] = r;
        return std::move(r); // ----->
    } else {
        return i->second; // ----->
    }
}


template<typename TResult, typename ... TArgs>
void LazyCacher<TResult, TArgs ... >::clean() {
    m.clean();
}


template<typename TResult, typename ... TArgs>
void LazyCacher<TResult, TArgs ... >::clean(TArgs const & ... args) {
    m.erase(TArgsTuple(args ...));
}


template<typename TResult, typename ... TArgs>
void LazyCacher<TResult, TArgs ... >::clean(TResult const &result) {
    auto i = m.begin();
    while (i != m.end()) {
        if (i->second == result)
            i = m.erase(i);
        else
            i++;
    }
}


template<typename TResult, typename ... TArgs>
typename LazyCacher<TResult, TArgs ... >::TArgsTupleResultMap
LazyCacher<TResult, TArgs ... >::m = LazyCacher<TResult, TArgs ... >::TArgsTupleResultMap();


} // caching
} // utility


#endif // HEADER_LAZY_CACHER_472B28A6_2137_4B4F_9C63_BAA8C459D633
