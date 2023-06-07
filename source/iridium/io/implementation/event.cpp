//#include "event.h"
//
//
//namespace iridium {
//namespace io {
//namespace implementation {
//
//
//CEvent::CEvent(IStream::TSharedPtr const &stream, TType const &type)
//:
//    m_stream(stream),
//    m_type  (type)
//{}
//
//
//IEvent::TType CEvent::getType() const {
//    return m_type;
//}
//
//
//void CEvent::setType(TType const &type) {
//    m_type = type;
//}
//
//
//IStream::TSharedPtr CEvent::getStream() const {
//    return m_stream;
//}
//
//
//} // implementation
//} // io
//} // iridium
