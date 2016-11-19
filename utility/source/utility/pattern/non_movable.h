



namespace utility {
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
} // utility
