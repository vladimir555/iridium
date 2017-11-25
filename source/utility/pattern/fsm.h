#ifndef HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34
#define HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34


#include "utility/smart_ptr.h"


namespace utility {
namespace pattern {


template<typename TState, typename TEvent>
class IFSM {
public:
    DEFINE_INTERFACE(IFSM)
    virtual TState doAction(TEvent const &event) = 0;
};


} // pattern
} // utility


#endif // HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34
