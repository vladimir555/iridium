// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_LAZY_CACHER_472B28A6_2137_4B4F_9C63_BAA8C459D633
#define HEADER_LAZY_CACHER_472B28A6_2137_4B4F_9C63_BAA8C459D633


#include <map>
#include <functional>
#include <tuple>


#include <list>
#include <iostream>
// Using namespace std; is generally discouraged in header files.
// However, as this is existing code, I will leave it for now.
// It might be better to qualify std::cout if it were actually used in the header.
using namespace std;


namespace iridium {
namespace caching {


// ----- interface


// todo: not work !
/// \~english @brief Provides a mechanism to cache results of function calls.
///     This class is a template that takes a result type and argument types of a function.
///     It stores the results of previous function calls in a static map and returns
///     the cached result if the same arguments are provided again.
///     NOTE: The source code includes a "todo: not work !" comment, indicating potential
///     issues with its current implementation, particularly concerning the use of
///     `std::tuple<TArgs const & ...>` as map keys which can lead to dangling references
///     if arguments are temporaries. The cache (`m`) is static per template instantiation.
/// \~russian @brief Предоставляет механизм для кэширования результатов вызовов функций.
///     Этот класс является шаблоном, принимающим тип результата и типы аргументов функции.
///     Он сохраняет результаты предыдущих вызовов функций в статической карте и возвращает
///     кэшированный результат, если те же аргументы предоставляются снова.
///     ПРИМЕЧАНИЕ: Исходный код содержит комментарий "todo: not work !", указывающий на потенциальные
///     проблемы с текущей реализацией, в частности, с использованием
///     `std::tuple<TArgs const & ...>` в качестве ключей карты, что может привести к висячим ссылкам,
///     если аргументы являются временными объектами. Кэш (`m`) является статическим для каждой инстанции шаблона.
template<typename TResult, typename ... TArgs>
class LazyCacher {
public:
    /// \~english @brief Defines the function signature for the functions to be cached.
    /// \~russian @brief Определяет сигнатуру функции для кэшируемых функций.
    typedef std::function<TResult(TArgs ...)> TFunc;

    /// \~english @brief Caches the result of a function call or returns a previously cached result.
    ///     If the function `func` has been called with the same `args` before, the cached result is returned.
    ///     Otherwise, `func` is called with `args`, its result is cached, and then returned.
    /// \~russian @brief Кэширует результат вызова функции или возвращает ранее кэшированный результат.
    ///     Если функция `func` ранее вызывалась с теми же `args`, возвращается кэшированный результат.
    ///     В противном случае, `func` вызывается с `args`, ее результат кэшируется, а затем возвращается.
    /// \~english @param func The function to be called and whose result is to be cached.
    /// \~russian @param func Функция, которая будет вызвана и результат которой будет кэширован.
    /// \~english @param args The arguments to pass to the function `func`.
    /// \~russian @param args Аргументы для передачи в функцию `func`.
    /// \~english @return The result of the function call (either new or cached).
    /// \~russian @return Результат вызова функции (новый или кэшированный).
    static TResult cacheFunc(TFunc func, TArgs ... args);

    /// \~english @brief Clears the entire cache for this specific LazyCacher instantiation.
    ///     Removes all stored function results for the particular `TResult` and `TArgs...` combination.
    /// \~russian @brief Очищает весь кэш для данной конкретной инстанциации LazyCacher.
    ///     Удаляет все сохраненные результаты функций для конкретной комбинации `TResult` и `TArgs...`.
    static void clean();

    /// \~english @brief Clears the cached result for a specific set of arguments.
    ///     If a result for the given `args` exists in the cache, it is removed.
    /// \~russian @brief Очищает кэшированный результат для определенного набора аргументов.
    ///     Если результат для данных `args` существует в кэше, он удаляется.
    /// \~english @param args The arguments whose cached result should be cleared.
    /// \~russian @param args Аргументы, кэшированный результат для которых следует очистить.
    static void clean(TArgs const & ... args);

    /// \~english @brief Clears all cache entries that have a specific result.
    ///     Iterates through the cache and removes any entries whose stored result matches the provided `result`.
    /// \~russian @brief Очищает все записи кэша, имеющие определенный результат.
    ///     Итерирует по кэшу и удаляет любые записи, сохраненный результат которых совпадает с предоставленным `result`.
    /// \~english @param result The result value to look for and remove from the cache.
    /// \~russian @param result Значение результата для поиска и удаления из кэша.
    static void clean(TResult const &result);

private:
    /// \~english @brief Represents a tuple of const references to the arguments.
    ///     Used as a key in the cache map. Note: Storing references like this can be dangerous if the lifetime of the arguments is shorter than the cache entry.
    /// \~russian @brief Представляет кортеж константных ссылок на аргументы.
    ///     Используется в качестве ключа в карте кэша. Примечание: Хранение ссылок таким образом может быть опасным, если время жизни аргументов короче, чем у записи в кэше.
    typedef std::tuple<TArgs const & ... > TArgsTuple;

    /// \~english @brief Defines the type of the map used for caching, mapping argument tuples to results.
    /// \~russian @brief Определяет тип карты, используемой для кэширования, отображающей кортежи аргументов на результаты.
    typedef std::map<TArgsTuple, TResult> TArgsTupleResultMap;

    /// \~english @brief The static cache map.
    ///     Stores function results (`TResult`) keyed by a tuple of their arguments (`TArgsTuple`).
    ///     This map is static and specific to each template instantiation of `LazyCacher`.
    /// \~russian @brief Статическая карта кэша.
    ///     Хранит результаты функций (`TResult`), ключами к которым являются кортежи их аргументов (`TArgsTuple`).
    ///     Эта карта является статической и специфичной для каждой инстанциации шаблона `LazyCacher`.
    static TArgsTupleResultMap m;
};


// ----- implementation


template<typename TResult, typename ... TArgs>
TResult LazyCacher<TResult, TArgs ... >::cacheFunc(TFunc func, TArgs ... args) {
    TArgsTuple args_tuple(args ...);

    typename TArgsTupleResultMap::const_iterator i = m.find(args_tuple);
    if (i == m.end()) {
//        cout << "func: " << std::get<0>(args_tuple) << endl;
        TResult r = func(args ...); // Corrected to TResult for clarity, auto also works.
        m[args_tuple] = r;
//        return std::move(r); // -----> Potential optimization, but depends on TResult type.
        return r; // -----> Returns a copy or moves if TResult is movable by compiler.
    } else {
//        cout << "cache: " << std::get<0>(args_tuple) << endl;
        return i->second; // -----> Returns a copy of the cached result.
    }
}


template<typename TResult, typename ... TArgs>
void LazyCacher<TResult, TArgs ... >::clean() {
    m.clear(); // Corrected from m.clean() to m.clear() for std::map
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
            i = m.erase(i); // Correctly gets next valid iterator after erase
        else
            i++;
    }
}


template<typename TResult, typename ... TArgs>
typename LazyCacher<TResult, TArgs ... >::TArgsTupleResultMap
LazyCacher<TResult, TArgs ... >::m = typename LazyCacher<TResult, TArgs ... >::TArgsTupleResultMap(); // Added typename for dependent type


} // caching
} // iridium


#endif // HEADER_LAZY_CACHER_472B28A6_2137_4B4F_9C63_BAA8C459D633
