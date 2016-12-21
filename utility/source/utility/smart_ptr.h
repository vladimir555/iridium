#ifndef HEADER_SMART_PTR_F399D98D_6C9C_4F01_A450_D336682C6F2A
#define HEADER_SMART_PTR_F399D98D_6C9C_4F01_A450_D336682C6F2A


#include <memory>


#define DEFINE_SMART_PTR(TClass) \
typedef std::shared_ptr<TClass>         TSharedPtr; \
typedef std::shared_ptr<TClass const>   TConstSharedPtr; \
typedef std::weak_ptr<TClass>           TWeakPtr; \
typedef std::weak_ptr<TClass const>     TConstWeakPtr


#define DEFINE_CREATE(TClass) \
template<typename ... TArgs> \
static std::shared_ptr<TClass> create(TArgs ... args) { \
    return std::make_shared<TClass>(args ...); \
}


#endif // HEADER_SMART_PTR_F399D98D_6C9C_4F01_A450_D336682C6F2A
