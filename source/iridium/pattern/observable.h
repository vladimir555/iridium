#ifndef HEADER_OBSERVABLE_EDD92C20_D8E9_491E_B074_1B31B94F754C
#define HEADER_OBSERVABLE_EDD92C20_D8E9_491E_B074_1B31B94F754C


#include "observer.h"


namespace iridium {
namespace pattern {


template<typename TEvent>
class IObservable {
public:
    DEFINE_INTERFACE(IObservable<TEvent>)
    virtual void subscribe	(typename IObserver<TEvent>::TSharedPtr const &observer) = 0;
    virtual void unsubscribe(typename IObserver<TEvent>::TSharedPtr const &observer) = 0;
};


} // pattern
} // iridium


#endif // HEADER_OBSERVABLE_EDD92C20_D8E9_491E_B074_1B31B94F754C
