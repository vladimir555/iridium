// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NON_COPYABLE_F54A924C_4ECF_486A_BEB7_5E749E8DC0BD
#define HEADER_NON_COPYABLE_F54A924C_4ECF_486A_BEB7_5E749E8DC0BD


namespace iridium::pattern {


///
class NonCopyable {
public:
    virtual ~NonCopyable() = default;
protected:
    NonCopyable() = default;
    NonCopyable(NonCopyable const &) = delete;
    NonCopyable &operator=(NonCopyable const &) const = delete;
};


} // namespace iridium::pattern


#endif // HEADER_NON_COPYABLE_F54A924C_4ECF_486A_BEB7_5E749E8DC0BD
