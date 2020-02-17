#ifndef HEADER_VA_ARGS_A200B22F_46BD_4D03_A488_82E71F3E5F7F
#define HEADER_VA_ARGS_A200B22F_46BD_4D03_A488_82E71F3E5F7F


namespace iridium {
namespace macros {
} // macros
} // iridium


// --- поддержка до 7 аргументов включительно
#define dFUNC_CHOOSER_7(_f1, _f2, _f3, _f4, _f5, _f6, _f7, N, ... ) N

#define dFUNC_RECOMPOSER(argsWithParentheses)\
    dFUNC_CHOOSER_7 argsWithParentheses

#define dMACRO_CHOOSER(target_, ...)\
    dCHOOSE_FROM_ARG_COUNT(target_, target_##_NO_ARG_EXPANDER __VA_ARGS__ ())

#define dCHOOSE_FROM_ARG_COUNT(arg_, ...) \
    dFUNC_RECOMPOSER((__VA_ARGS__, arg_##_7, arg_##_6, arg_##_5, arg_##_4, arg_##_3, arg_##_2, arg_##_1, ))

// -----

/*
//// реализации перегруженного макроса под различное количество аргументов
//#define dACTION_7(a1, a2, a3, a4, a5, a6, a7) \
//    std::cout << "7:ACTION(" << a1 << "," << a2 << "," << a3 << "," << a4 << ","<< a5 << "," << a6 << "," << a7 << ")\n"

//#define dACTION_6(a1, a2, a3, a4, a5, a6) \
//    std::cout << "6:ACTION(" << a1 << "," << a2 << "," << a3 << "," << a4 << ","<< a5 << "," << a6 << ")\n"

//#define dACTION_5(a1, a2, a3, a4, a5) \
//    std::cout << "5:ACTION(" << a1 << "," << a2 << "," << a3 << "," << a4 << ","<< a5 << ")\n"

//#define dACTION_4(a1, a2, a3, a4) \
//    std::cout << "4:ACTION(" << a1 << "," << a2 << "," << a3 << "," << a4 << ")\n"

//#define dACTION_3(a1, a2, a3) \
//    std::cout << "3:ACTION(" << a1 << "," << a2 << "," << a3 << ")\n"

//#define dACTION_2(a1, a2) \
//    std::cout << "2:ACTION(" << a1 << "," << a2 << ")\n"

//#define dACTION_1(a1) \
//    std::cout << "1:ACTION(" << a1 << ")\n"

//#define dACTION_0() \
//    std::cout << "0:ACTION()\n"


//#define dACTION_NO_ARG_EXPANDER() \
//    ,,,,,,,dACTION_0

//#define ACTION(...)\
//    dMACRO_CHOOSER( dACTION, __VA_ARGS__)(__VA_ARGS__)


//#include <iostream>

//int main()
//{
//    std::cout << "Hello, world!\n";

//    ACTION();
//    ACTION(1);
//    ACTION(1,2);
//    ACTION(1,2,3);
//    ACTION(1,2,3,4);
//    ACTION(1,2,3,4,5);
//    ACTION(1,2,3,4,5,6);
//    ACTION(1,2,3,4,5,6,7);
//}
*/


#endif // HEADER_VA_ARGS_A200B22F_46BD_4D03_A488_82E71F3E5F7F
