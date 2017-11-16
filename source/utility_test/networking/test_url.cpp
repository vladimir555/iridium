#include <gtest/gtest.h>
#include <iostream>

#include <utility/networking/url.h>
#include <utility/convertion/convert.h>


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

    ASSERT_NO_FATAL_FAILURE(URL("http://ya.ru").getIPv4());
    ASSERT_LE(7, URL("http://ya.ru").getIPv4AsString().size());
}


} // networking
} // utility
