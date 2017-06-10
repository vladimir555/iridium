#ifndef HEADER_SMART_PTR_C1E6B022_A228_47C3_9E76_E3C64E43617E
#define HEADER_SMART_PTR_C1E6B022_A228_47C3_9E76_E3C64E43617E


#include <memory>


#define DEFINE_SMART_PTR(TClass) \
typedef std::shared_ptr<TClass>         TSharedPtr; \
typedef std::weak_ptr  <TClass>         TWeakPtr; \
typedef std::shared_ptr<TClass const>   TConstSharedPtr; \
typedef std::weak_ptr  <TClass const>   TConstWeakPtr;


#define DEFINE_CREATE(TClass) \
template<typename ... TArgs> \
static std::shared_ptr<TClass> create(TArgs && ... args) { \
    return std::make_shared<TClass>(args ...); \
}


#endif // HEADER_SMART_PTR_C1E6B022_A228_47C3_9E76_E3C64E43617E
