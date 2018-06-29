#ifndef HEADER_CACHE_F566D6D8_C42D_4049_9642_C600B057A128
#define HEADER_CACHE_F566D6D8_C42D_4049_9642_C600B057A128


#include "utility/smart_ptr.h"
#include "utility/pattern/initializable.h"

#include <string>
#include <list>


namespace utility {
namespace io {
namespace fs {


class ICache: public pattern::IInitializable {
public:
    DEFINE_SMART_PTR(ICache)
    /// destructor
    virtual ~ICache() = default;
    /// insert value, return value id
    virtual size_t push(std::string const &line) = 0;
    /// get value by id
    virtual std::string get(size_t const &id) const = 0;
    /// remove value by id
    virtual void remove(size_t const &id) = 0;
    /// remove all values
    virtual void clean() = 0;
    /// get cache size
    virtual size_t size() const = 0;
    /// 
    virtual bool checkExistense(size_t const &id) const = 0;
    ///
    virtual void flush() = 0;
};


} // io
} // fs
} // utility


#endif // HEADER_CACHE_F566D6D8_C42D_4049_9642_C600B057A128
