#include "ssl.h"



namespace iridium {
namespace encryption {
namespace implementation {


CContextDefault::CContextDefault(
        bool        const &,
        std::string const &,
        std::string const &)
{
    throw std::runtime_error("SSL not implemented"); // ----->
}


ISSL::TSharedPtr CContextDefault::accept(int const &) {
    throw std::runtime_error("SSL not implemented"); // ----->
}


CContextDefault::CSSL::CSSL(IContext::TSharedPtr const &, int const &, bool const &) {
    throw std::runtime_error("SSL not implemented"); // ----->
}


size_t CContextDefault::CSSL::write(io::Buffer::TSharedPtr const &) {
    throw std::runtime_error("SSL not implemented"); // ----->
}


io::Buffer::TSharedPtr CContextDefault::CSSL::read(size_t const &) {
    throw std::runtime_error("SSL not implemented"); // ----->
}


int CContextDefault::CSSL::getID() const {
    throw std::runtime_error("SSL not implemented"); // ----->
}


void CContextDefault::CSSL::initialize() {
    throw std::runtime_error("SSL not implemented"); // ----->
}


void CContextDefault::CSSL::finalize() {
    throw std::runtime_error("SSL not implemented"); // ----->
}


} // implementation
} // encryption
} // iridium
