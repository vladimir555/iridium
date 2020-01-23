#pragma once


#include "iridium/smart_ptr.h"


namespace iridium {
namespace pattern {


template<typename TEvent>
class IObservable;


template<typename TEvent>
class IObserver {
public:
	DEFINE_INTERFACE(IObserver<TEvent>)
	virtual void handle(
		TEvent const& event,
		typename IObservable<TEvent>::TSharedPtr const &observable) = 0;
};


} // pattern
} // iridium
