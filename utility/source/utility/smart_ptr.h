#include <memory>


#define DEFINE_SMART_PTR(TClass) \
typedef std::shared_ptr<TClass>         TSharedPtr; \
typedef std::shared_ptr<TClass const>   TConstSharedPtr; \
typedef std::weak_ptr<TClass>           TWeakPtr; \
typedef std::weak_ptr<TClass const>     TConstWeakPtr


#define DEFINE_CREATE(TClass)
template<typename ... TArgs>
std::shared_ptr<TClass> create(TArgs ... args) {
    return std::make_shared<TClass>(args ...);
}
