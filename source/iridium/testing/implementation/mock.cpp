//#include "mock.h"
//
//#include "iridium/convertion/convert.h"
//
//
//using std::string;
//using iridium::convertion::convert;
//
//
//namespace iridium {
//namespace testing {
//namespace implementation {
//
//
//void CMock::setupBehavior(
//    string const &method_name,
//    std::function<void()> const &behavior)
//{
//    m_map_name_behavior[method_name] = behavior;
//}
//
//
//void CMock::callMethod(string const &method_name) {
//    if (m_map_name_behavior.find(method_name) == m_map_name_behavior.end()) {
//        throw std::runtime_error("unexpected method call: " + method_name);
//    }
//    m_map_name_behavior  [method_name]();
//    m_map_name_call_count[method_name]++;
//}
//
//
//void CMock::verifyCall(
//    string const &method_name,
//    int const &expected_calls) const
//{
//    auto name_call_count = m_map_name_call_count.find(method_name);
//    if (name_call_count == m_map_name_call_count.end() || name_call_count->second != expected_calls) {
//        throw std::runtime_error(
//            "method " + method_name + " was called " +
//            convert<string>(name_call_count->second) + " times, but expected " +
//            convert<string>(expected_calls)); // ----->
//    }
//}
//
//
//} // implementation
//} // testing
//} // iridium
