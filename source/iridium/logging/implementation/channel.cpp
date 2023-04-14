//#include "channel.h"
//#include "iridium/pattern/initializable.h"
//
//
//using iridium::pattern::IInitializable;
//
//
//namespace iridium {
//namespace logging {
//namespace implementation {
//
//
//void CChannel::initialize() {
////    LOCK_SCOPE();
//    broadcast(&ISink::initialize);
//}
//
//
//void CChannel::finalize() {
////    LOCK_SCOPE();
//    broadcast(&ISink::finalize);
//}
//
//
//void CChannel::attach(ISink::TSharedPtr const &sink) {
////    LOCK_SCOPE();
//    Broadcaster<ISink>::attach(sink);
//}
//
//
//void CChannel::detach(ISink::TSharedPtr const &sink) {
////    LOCK_SCOPE();
//    Broadcaster<ISink>::detach(sink);
//}
//
//
//void CChannel::log(TEvent &&e) {
////    LOCK_SCOPE();
//    broadcast(&ISink::log, e);
//}
//
//
//} // implementation
//} // logger
//} // iridium
