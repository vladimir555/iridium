// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SMART_PTR_C1E6B022_A228_47C3_9E76_E3C64E43617E
#define HEADER_SMART_PTR_C1E6B022_A228_47C3_9E76_E3C64E43617E


#include <memory>


#define DEFINE_SMART_PTR(TClass) \
typedef ::std::shared_ptr<TClass>         TSharedPtr; \
typedef ::std::weak_ptr  <TClass>         TWeakPtr; \
typedef ::std::unique_ptr<TClass>         TUniquePtr; \
typedef ::std::shared_ptr<TClass const>   TConstSharedPtr; \
typedef ::std::weak_ptr  <TClass const>   TConstWeakPtr; \
typedef ::std::unique_ptr<TClass const>   TConstUniquePtr;


#ifdef DEFINE_MOCK_CREATE
#include "iridium/testing/mock.h"
#endif // DEFINE_MOCK_CREATE


#ifdef DEFINE_MOCK_CREATE

#define DEFINE_CREATE(Interface) \
DEFINE_SMART_PTR(Interface) \
template<typename ... TArgs> \
static auto create(TArgs && ... args) { \
return ::iridium::testing::Mock<Interface>::create(std::forward<TArgs>(args) ...); \
};

#else

#define DEFINE_CREATE(TClass) \
DEFINE_SMART_PTR(TClass) \
template<typename ... TArgs> \
static ::std::shared_ptr<TClass> create(TArgs && ... args) { \
    return ::std::make_shared<TClass>(std::forward<TArgs>(args) ...); \
};

#endif // DEFINE_MOCK_CREATE


#define DEFINE_INTERFACE(TClass) \
DEFINE_SMART_PTR(TClass) \
virtual ~TClass() = default;

#define DEFINE_IMPLEMENTATION(TClass) \
DEFINE_CREATE(TClass) \
virtual ~TClass() override = default;


#endif // HEADER_SMART_PTR_C1E6B022_A228_47C3_9E76_E3C64E43617E
