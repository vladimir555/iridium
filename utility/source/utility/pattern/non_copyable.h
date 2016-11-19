



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
