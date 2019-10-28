#include <iridium/enum.h>



namespace iridium {
namespace pattern {
namespace test {


class Light {
public:
    Light() = default;
    virtual ~Light() = default;

    DEFINE_ENUM(
        TState,
        LIGHT_OFF,
        LIGHT_ON
    )

    DEFINE_ENUM(
        TEvent,
        PUSH_BUTTON
    )

    template<TEvent::TEnumInternal, TState::TEnumInternal, TState::TEnumInternal>
    void handle();

    TState m_state = TState::LIGHT_OFF;
    TState doAction(TEvent const &event);


    int i = 0;
};


} // test
} // pattern
} // iridium
