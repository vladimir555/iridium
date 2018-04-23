#include <gtest/gtest.h>
#include <iostream>

#include <utility/networking/url.h>
#include <utility/networking/dns.h>
#include <utility/logging/logger.h>
#include <utility/networking/implementation/platform/windows/wsa.h>
#include <utility/convertion/convert.h>
#include <utility/assert.h>


using namespace std;
using utility::convertion::convert;


namespace utility {
namespace networking {


TEST(networking, url) {
    ASSERT_THROW(URL(""), std::exception);
    ASSERT_THROW(URL("172.16.0.64"), std::exception);
    ASSERT_THROW(URL("55555"), std::exception);
    ASSERT_THROW(URL(":55555"), std::exception);
    ASSERT_THROW(URL("::"), std::exception);

    URL url("https://172.16.0.64:55555");

    ASSERT_TRUE(static_cast<bool>(url.getHost()));
    ASSERT_TRUE(static_cast<bool>(url.getIPv4()));
    ASSERT_TRUE(static_cast<bool>(url.getPort()));
    ASSERT_TRUE(static_cast<bool>(url.getProtocol()));

    ASSERT_EQ(URL::TProtocol::HTTPS, url.getProtocol());
    ASSERT_EQ(std::vector<uint8_t>({ 172, 16, 0, 64 }), *url.getIPv4());
    ASSERT_EQ("172.16.0.64", url.getIPv4AsString());
    ASSERT_EQ(55555, *url.getPort());

    url = convert<URL>(string("http://hostname.ru"));
    ASSERT_TRUE(static_cast<bool>(url.getHost()));
    ASSERT_EQ("hostname.ru", *url.getHost());

    try {
        logging::update(logging::config::createDefaultConsoleLoggerConfig());
        //bool b = URL("http://ya.ru").getIPv4() == nullptr;
        //ASSERT_TRUE(b);
        //cout << URL("http://ya.ru").getIPv4AsString() << endl;
        //ASSERT_LE(static_cast<size_t>(7), URL("http://ya.ru").getIPv4AsString().size());
        //implementation::platform::WSA::instance();        
        //cout << getIPv4ByHost("ya.ru").size() << endl;
        //cout << URL("http://ya.ru").getIPv4AsString() << endl;
    } catch (std::exception const &e) {
        cout << e.what() << endl;
        FAIL();
    }
}


} // networking
} // utility
