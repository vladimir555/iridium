#ifndef HEADER_CONTEXT_5E470497_AC46_415C_B3E2_7003AE1D21F6
#define HEADER_CONTEXT_5E470497_AC46_415C_B3E2_7003AE1D21F6


#include "event.h"


namespace iridium {
namespace io {


class IContext {
public:
    DEFINE_INTERFACE(IContext)
    
    virtual void pushEvent(Event::TSharedPtr const &event) = 0;
    virtual std::list<Event::TSharedPtr>
                 popEvents() = 0;
    virtual std::list<Event::TSharedPtr>
                 checkOutdatedStreams() = 0;
    virtual bool update  (Event::TSharedPtr const &event) = 0;
    virtual bool transmit(Event::TSharedPtr const &event) = 0;
    virtual void remove  () = 0;
};


class IPipeManager {
public:
    DEFINE_INTERFACE(IPipeManager)
    
    virtual void createPipe(std::string const &name) = 0;
    virtual void removePipe(std::string const &name) = 0;
    virtual void updatePipe(std::string const &name, IStreamReader::TSharedPtr const &reader) = 0;
    virtual void updatePipe(std::string const &name, IStreamWriter::TSharedPtr const &writer) = 0;
    virtual void updatePipe(std::string const &name, IStreamReader::TSharedPtr const &reader, IStreamWriter::TSharedPtr const &writer) = 0;
};


} // io
} // iridium


#endif // HEADER_CONTEXT_5E470497_AC46_415C_B3E2_7003AE1D21F6
