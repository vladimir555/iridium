#ifndef HEADER_NON_COPYABLE_F54A924C_4ECF_486A_BEB7_5E749E8DC0BD
#define HEADER_NON_COPYABLE_F54A924C_4ECF_486A_BEB7_5E749E8DC0BD


namespace utility {
namespace pattern {


///
class NonCopyable {
public:
    virtual ~NonCopyable() = default;
protected:
    ///
    NonCopyable() = default;
    ///
    NonCopyable(NonCopyable const &) = delete;
    ///
    NonCopyable &operator=(NonCopyable const &) const = delete;
};


} // pattern
} // utility


#endif // HEADER_NON_COPYABLE_F54A924C_4ECF_486A_BEB7_5E749E8DC0BD
