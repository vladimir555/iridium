// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_MOCK_REGISTRY_4EB17BD7_79AF_4F95_B6B2_A331B4ABA19A
#define HEADER_MOCK_REGISTRY_4EB17BD7_79AF_4F95_B6B2_A331B4ABA19A


//#ifdef DEFINE_MOCK_CREATE


#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <list>


namespace iridium {
namespace testing {


/// TClass as TClass or Mock<TClass>
template<typename TClass>
class MockRegistry {
public:
    /// default TClass constructor without mock
    template<typename ... TArgs>
    static std::shared_ptr<TClass> take(TArgs&& ... args);
    static void add(TClass * const mock);
};


template<typename TClass>
class Mock;


template<typename TClass>
class MockRegistryBase {
public:
    static void add(TClass * const mock);
protected:
    template<typename TClassMock>
    static std::shared_ptr<TClassMock> take();
private:
    using TMock = ::iridium::testing::Mock<TClass>;
    static std::unordered_map<std::type_info const *, std::list<TMock *> > m_map_name_mock;
};


// implementation


template<typename TClass>
template<typename ... TArgs>
std::shared_ptr<TClass> MockRegistry<TClass>::take(TArgs && ... args) {
    return std::make_shared<TClass>(std::forward<TArgs>(args)...);
}


template<typename TClass>
void MockRegistry<TClass>::add(TClass * const) {}


template<typename TClass>
void MockRegistryBase<TClass>::add(TClass * const mock) {
    printf("mock registry add: %s", typeid(TClass).name());
    m_map_name_mock[&typeid(TClass)].push_back(dynamic_cast<TMock *>(mock));
}


template<typename TClass>
template<typename TClassMock>
std::shared_ptr<TClassMock> MockRegistryBase<TClass>::take() {
    printf("mock registry take mock: %s", typeid(TClass).name());
    auto name_mock = m_map_name_mock.find(&typeid(TClass));
    if (name_mock == m_map_name_mock.end() || name_mock->second.empty()) {
        throw std::runtime_error(
            "getting regeistered mock of class '" + std::string(typeid(TClass).name()) +
            "' object error: not enough such registered mock objects");
    } else {
        auto mock = *name_mock->second.begin();
        name_mock->second.pop_front();
        // shared_ptr without destructor
        return std::shared_ptr<TClassMock>(dynamic_cast<TClassMock *>(mock), [] (TClassMock *) {});
    }
}


} // testing
} // iridium


//template<typename TClass>
//using MockRegistry = ::iridium::testing::MockRegistry<TClass>;


// #endif // DEFINE_MOCK_CREATE


#endif // HEADER_MOCK_REGISTRY_4EB17BD7_79AF_4F95_B6B2_A331B4ABA19A
