#ifndef HEADER_INITIALIZER_A79AA8C7_D7C5_492D_A771_2633A8E098DA
#define HEADER_INITIALIZER_A79AA8C7_D7C5_492D_A771_2633A8E098DA


#include "iridium/pattern/initializable.h"

#include <list>


namespace iridium {
namespace pattern {
namespace implementation {


class CInitializer: public IInitializable {
public:
    DEFINE_CREATE(CInitializer)
//    CInitializer(std::list<IInitializable::TSharedPtr> const &items);
    template<typename ... TArgs>
    CInitializer(TArgs const & ... items);
    virtual ~CInitializer();

    void initialize()   override;
    void finalize()     override;

private:
    bool                                    m_is_initialized;
    std::list<IInitializable::TSharedPtr>   m_items;
};


template<typename ... TArgs>
CInitializer::CInitializer(TArgs const & ... items)
:
    m_is_initialized    (false),
    m_items             {items ...}
{
    for (auto const &item: m_items)
        if (!item)
            throw std::runtime_error("create initializer error: nullptr item"); // ----->
}


} // implementation
} // pattern
} // iridium


#endif // HEADER_INITIALIZER_A79AA8C7_D7C5_492D_A771_2633A8E098DA
