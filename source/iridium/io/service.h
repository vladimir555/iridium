#ifndef HEADER_SERVICE_35D1546E_2618_4473_8380_19C506162205
#define HEADER_SERVICE_35D1546E_2618_4473_8380_19C506162205


//#include "iridium/io/listener.h"
//#include "iridium/pattern/singleton.h"
//#include "iridium/threading/synchronized.h"

//#include <atomic>


//namespace iridium {
//namespace io {


//class Service:
//    public IListener,
//    public pattern::Singleton<Service>,
//    public threading::Synchronized
//{
//public:
//    void    add(IStream::TSharedPtr const &stream) override;
//    void    del(IStream::TSharedPtr const &stream) override;
//    TEvents wait() override;

//private:
//    friend class pattern::Singleton<Service>;

//    Service();
//    virtual ~Service();

//    void    initialize() override;
//    void    finalize() override;

//    IListener::TSharedPtr   m_listener;
//    std::atomic<bool>       m_is_initialized;
//};


//} // io
//} // iridium


#endif // HEADER_SERVICE_35D1546E_2618_4473_8380_19C506162205
