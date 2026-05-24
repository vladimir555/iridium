#ifndef HEADER_OBSERVABLE_FE9A8D01_3845_472A_8A6E_B413B9C5FB6C
#define HEADER_OBSERVABLE_FE9A8D01_3845_472A_8A6E_B413B9C5FB6C


#include "iridium/pattern/observable.h"
#include "iridium/threading/synchronized.h"

#include <list>
#include <mutex>
#include <algorithm>


namespace iridium::pattern::implementation {


template<typename TEvent>
class CObservable:
    public threading::Synchronized<std::mutex>,
    public IObservable<TEvent>
{
public:
    DEFINE_IMPLEMENTATION(CObservable)
    void subscribe  (typename IObserver<TEvent>::TWeakPtr const &observer) override;
    void unsubscribe(typename IObserver<TEvent>::TWeakPtr const &observer) override;
    void notify     (TEvent const &event) override;

private:
    std::list<typename IObserver<TEvent>::TWeakPtr> getLiveObservers();
    std::list<typename IObserver<TEvent>::TWeakPtr>
        m_observers;
    std::mutex
        m_mutex;
};


// implementation


template<typename TEvent>
void CObservable<TEvent>::subscribe(typename IObserver<TEvent>::TWeakPtr const &observer) {
    LOCK_SCOPE();
    m_observers.push_back(observer);
}

template<typename TEvent>
void CObservable<TEvent>::unsubscribe(typename IObserver<TEvent>::TWeakPtr const &observer) {
    LOCK_SCOPE();
    auto target = observer.lock();

    m_observers.erase(
        std::remove_if(
            m_observers.begin(),
            m_observers.end(),
            [&target](const auto& weakObs) {
                auto obs = weakObs.lock();
                return obs && obs == target;
            }
        ),
        m_observers.end()
    );
}

template<typename TEvent>
void CObservable<TEvent>::notify(TEvent const &event) {
    auto live_observers = getLiveObservers();

    // close expired
    for (auto &weak_observer : live_observers) {
        if (auto observer = weak_observer.lock())
            observer->handleEvent(event);
    }
}

template<typename TEvent>
std::list<typename IObserver<TEvent>::TWeakPtr> CObservable<TEvent>::getLiveObservers() {
    LOCK_SCOPE();

    m_observers.erase(
        std::remove_if(
            m_observers.begin(),
            m_observers.end(),
            [](const auto& weak_object) {
                return weak_object.expired();
            }
        ),
        m_observers.end()
    );

    return m_observers;
}


} // iridium::pattern::implementation


#endif // HEADER_OBSERVABLE_FE9A8D01_3845_472A_8A6E_B413B9C5FB6C
