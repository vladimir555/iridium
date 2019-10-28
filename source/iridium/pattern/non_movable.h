#ifndef HEADER_NON_MOVABLE_8C6763E9_B57A_4D83_93DD_AF9608B7E11A
#define HEADER_NON_MOVABLE_8C6763E9_B57A_4D83_93DD_AF9608B7E11A


namespace iridium {
namespace pattern {


///
class NonMovable {
public:
    virtual ~NonMovable() = default;
protected:
    ///
    NonMovable() = default;
    ///
    NonMovable(NonMovable const &&) = delete;
//    ///
//    NonMovable &operator=(NonMovable const &) const = delete;
};


} // pattern
} // iridium


#endif // HEADER_NON_MOVABLE_8C6763E9_B57A_4D83_93DD_AF9608B7E11A
