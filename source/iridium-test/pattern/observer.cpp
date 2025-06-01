#include <iridium/testing/tester.h>

#include "iridium/pattern/implementation/observable.h"


using iridium::pattern::IObserver;
using iridium::pattern::IObservable;
using iridium::pattern::implementation::CObservable;


struct ButtonClickEvent {
    int x;
    int y;
};


class MockObserver : public IObserver<ButtonClickEvent> {
public:
    DEFINE_IMPLEMENTATION(MockObserver)
    void handleEvent(ButtonClickEvent const &event) override {
        last_event_ = event;
        call_count_++;
    }

    ButtonClickEvent last_event_{};
    int call_count_ = 0;
};


TEST(observer_basic_notification) {
    auto observable = CObservable<ButtonClickEvent>::create();
    auto observer   = MockObserver::create();

    observable->subscribe(observer);
    observable->notify(ButtonClickEvent{10, 20});

    ASSERT(1,   equal, observer->call_count_);
    ASSERT(10,  equal, observer->last_event_.x);
    ASSERT(20,  equal, observer->last_event_.y);
}


TEST(observer_unsubscribe_works) {
    auto observable = CObservable<ButtonClickEvent>::create();
    auto observer   = MockObserver::create();

    observable->subscribe(observer);
    observable->unsubscribe(observer);
    observable->notify(ButtonClickEvent{});

    ASSERT(0, equal, observer->call_count_);
}


TEST(observer_multiple_observers) {
    auto observable = CObservable<ButtonClickEvent>::create();
    auto observer1  = MockObserver::create();
    auto observer2  = MockObserver::create();

    observable->subscribe(observer1);
    observable->subscribe(observer2);
    observable->notify(ButtonClickEvent{30, 40});

    ASSERT(1,   equal, observer1->call_count_);
    ASSERT(1,   equal, observer2->call_count_);
    ASSERT(30,  equal, observer1->last_event_.x);
}
