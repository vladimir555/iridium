#pragma once


#include "iridium/smart_ptr.h"
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
