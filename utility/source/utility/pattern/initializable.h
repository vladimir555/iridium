#ifndef HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
#define HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08


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


#endif // HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
