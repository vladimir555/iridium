#ifndef HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34
#define HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34


#include "iridium/smart_ptr.h"


namespace iridium {
namespace pattern {


template<typename TEvent, typename TState>
class IFSM {
public:
    DEFINE_INTERFACE(IFSM)
    virtual TState doAction(TEvent const &event) = 0;
};


} // pattern
} // iridium


#endif // HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34
