



namespace utility {
namespace pattern {


///
class IInitializable {
public:
    virtual ~IInitializable() = default;
    ///
    virtual void initialize() = 0;
    ///
    virtual void finalize() = 0;
};


} // pattern
} // utility
