#ifndef HEADER_SERVICE_35D1546E_2618_4473_8380_19C506162205
#define HEADER_SERVICE_35D1546E_2618_4473_8380_19C506162205


#include "iridium/io/listener.h"
#include "iridium/pattern/singleton.h"


namespace iridium {
namespace io {


class Service: public IListener, public pattern::Singleton<Service> {
public:
//    friend class pattern::Singleton<Service>;

    Service();
    virtual ~Service();

    void    add(IStream::TSharedPtr const &stream) override;
    void    del(IStream::TSharedPtr const &stream) override;
    TEvents wait() override;

private:
    void    initialize() override;
    void    finalize() override;
    threading::IMutex::TSharedPtr getMutex() const override;

    IListener::TSharedPtr m_listener;
};


} // io
} // iridium


#endif // HEADER_SERVICE_35D1546E_2618_4473_8380_19C506162205
