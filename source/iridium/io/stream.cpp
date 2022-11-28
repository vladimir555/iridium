/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "stream.h"


////
//bool operator < (IStream::TSharedPtr const &l, IStream::TSharedPtr const &r) {
//    return !(l && r && (l->getID() < r->getID()));
//}


//bool operator < (IStream::TSharedPtr const &l, IStream::TConstSharedPtr const &r) {
//    return !(l && r && (l->getID() < r->getID()));
//}


//bool operator < (IStream::TSharedPtr const &l, IStream::TWeakPtr const &r_) {
//    auto r = r_.lock();
//    return !(l && r && (l->getID() < r->getID()));
//}


//bool operator < (IStream::TSharedPtr const &l, IStream::TConstWeakPtr const &r_) {
//    auto r = r_.lock();
//    return !(l && r && (l->getID() < r->getID()));
//}


////
//bool operator < (IStream::TConstSharedPtr const &l, IStream::TSharedPtr const &r) {
//    return !(l && r && (l->getID() < r->getID()));
//}


//bool operator < (IStream::TConstSharedPtr const &l, IStream::TConstSharedPtr const &r) {
//    return !(l && r && (l->getID() < r->getID()));
//}


//bool operator < (IStream::TConstSharedPtr const &l, IStream::TWeakPtr const &r_) {
//    auto r = r_.lock();
//    return !(l && r && (l->getID() < r->getID()));
//}


//bool operator < (IStream::TConstSharedPtr const &l, IStream::TConstWeakPtr const &r_) {
//    auto r = r_.lock();
//    return !(l && r && (l->getID() < r->getID()));
//}


////
//bool operator < (IStream::TWeakPtr const &l_, IStream::TSharedPtr const &r) {
//    auto l = l_.lock();
//    return !(l && r && (l->getID() < r->getID()));
//}


//bool operator < (IStream::TWeakPtr const &l_, IStream::TConstSharedPtr const &r) {
//    auto l = l_.lock();
//    return !(l && r && (l->getID() < r->getID()));
//}


//bool operator < (IStream::TWeakPtr const &l_, IStream::TWeakPtr const &r_) {
//    auto l = l_.lock();
//    auto r = r_.lock();
//    return !(l && r && (l->getID() < r->getID()));
//}


//bool operator < (IStream::TWeakPtr const &l_, IStream::TConstWeakPtr const &r_) {
//    auto l = l_.lock();
//    auto r = r_.lock();
//    return !(l && r && (l->getID() < r->getID()));
//}


////
//bool operator < (IStream::TConstWeakPtr const &l_, IStream::TSharedPtr const &r) {
//    auto l = l_.lock();
//    return !(l && r && (l->getID() < r->getID()));
//}


//bool operator < (IStream::TConstWeakPtr const &l_, IStream::TConstSharedPtr const &r) {
//    auto l = l_.lock();
//    return !(l && r && (l->getID() < r->getID()));
//}


//bool operator < (IStream::TConstWeakPtr const &l_, IStream::TWeakPtr const &r_) {
//    auto l = l_.lock();
//    auto r = r_.lock();
//    return !(l && r && (l->getID() < r->getID()));
//}


//bool operator < (IStream::TConstWeakPtr const &l_, IStream::TConstWeakPtr const &r_) {
//    auto l = l_.lock();
//    auto r = r_.lock();
//    return !(l && r && (l->getID() < r->getID()));
//}


//} // io
//} // iridium


//std::size_t std::hash<iridium::io::IStream::TSharedPtr>::operator()(iridium::io::IStream::TSharedPtr const &stream) const {
//    return stream->getID(); // ----->
//}
