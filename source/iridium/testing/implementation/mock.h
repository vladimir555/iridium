//#ifndef HEADER_MOCK_517A6549_8CF3_4DDE_ADD7_2218CC4A052F
//#define HEADER_MOCK_517A6549_8CF3_4DDE_ADD7_2218CC4A052F
//
//
//#include "iridium/testing/mock.h"
//
//#include <unordered_map>
//
//
//namespace iridium {
//namespace testing {
//namespace implementation {
//
//
//class CMock: public IMock {
//public:
//    DEFINE_IMPLEMENTATION(CMock)
//    void setupBehavior  (std::string const &method_name, std::function<void()> const &behavior) override;
//    void callMethod     (std::string const &method_name) override;
//    void verifyCall     (std::string const &method_name, int const &expected_calls = 1) const override;
//
//private:
//    std::unordered_map<std::string, std::function<void()>> m_map_name_behavior;
//    mutable std::unordered_map<std::string, int> m_map_name_call_count;
//};
//
//
//} // implementation
//} // testing
//} // iridium
//
//
//#endif // HEADER_MOCK_517A6549_8CF3_4DDE_ADD7_2218CC4A052F
