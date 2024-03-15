// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_THREAD_EC50D1AB_7469_400A_90D9_4977B01B0A71
#define HEADER_THREAD_EC50D1AB_7469_400A_90D9_4977B01B0A71


#include "iridium/pattern/initializable.h"
#include "iridium/smart_ptr.h"

#include <string>
#include <chrono>
#include <thread>


namespace iridium {
namespace threading {


///
class IThread: 
    public pattern::IInitializable
{
public:
    DEFINE_SMART_PTR(IThread)
    ///
    virtual ~IThread() = default;
    /// for std::map caching
    virtual bool operator < (IThread const &thread) const = 0;
    ///
    virtual std::string getName() const = 0;
    ///
    static std::string const &getNameStatic();
protected:
    ///
    static void setNameStatic(std::string const &name);
private:
    ///
    static thread_local std::string thread_name_static;
    
    
//    static std::__thread_id const thread_main_id;
//    static std::__thread_id initMainThreadID();
};


void sleep(int const &milliseconds);
std::string getThreadID();


} // threading
} // iridium


#endif // HEADER_THREAD_EC50D1AB_7469_400A_90D9_4977B01B0A71
