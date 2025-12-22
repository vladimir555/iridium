#ifndef HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA
#define HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/multiplexer.h"
#include "iridium/threading/synchronized.h"
#include "iridium/threading/async_queue.h"

#include <unordered_map>
#include <mutex>

#define NOMINMAX
#include <Windows.h>


namespace iridium::io::implementation::platform {


class CMultiplexer : public IMultiplexer, public threading::Synchronized<std::mutex> {
public:
    CMultiplexer();
    DEFINE_IMPLEMENTATION(CMultiplexer)

    void initialize() override;
    void finalize() override;
    std::list<Event::TSharedPtr> waitEvents() override;
    void   subscribe(IStream::TSharedPtr const &stream) override;
    void unsubscribe(IStream::TSharedPtr const &stream) override;
    void wake(Event::TSharedPtr const &event) override;
    void wake(std::list<Event::TSharedPtr> const &events) override;

private:
    DWORD checkResult(bool const &is_ok, std::string const &message);

    HANDLE m_iocp;
    std::unordered_map<HANDLE, IStream::TSharedPtr> m_map_id_stream;

    threading::IAsyncQueue<Event::TSharedPtr>::TSharedPtr m_wake_events;
};


} // iridium::io::implementation::platform


#endif // WINDOWS_PLATFORM


#endif // HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA
