#include <iridium/testing/tester.h>
#include <iridium/io/uri.h>


using std::string;


namespace iridium {
namespace io {


TEST(uri) {
    using iridium::io::URI;

    ASSERT("localhost" , equal, URI("localhost").getPath());
    ASSERT("localhost@", equal, URI("localhost%40").getPath());

    ASSERT(URI("")              , std::exception);
    ASSERT(URI("http://:55555") , std::exception);
    ASSERT(URI("http://::")     , std::exception);

    ASSERT(URI("postgres://user@1:password@localhost/database"), std::exception);

    ASSERT("arg1=1&arg2=2", equal, URI("http://user:password@localhost/path1/path2?arg1=1&arg2=2").getArguments());

    ASSERT("user@1", equal, URI("postgres://user%401:password@localhost/database").getUser());
    ASSERT("user@1", equal, URI("postgres://user%401@localhost/database").getUser());

    URI uri("https://172.16.0.64:55555");

    ASSERT(!uri.getHost().empty());
    ASSERT(static_cast<bool>(uri.getIPv4()));
    ASSERT(uri.getPort());

    ASSERT(URI::TProtocol::HTTPS, equal, uri.getProtocol());
    //    ASSERT({ 172, 16, 0, 64 }, equal, uri.getIPv4());
    //    LOGT << uri.getIPv4();
    ASSERT("172.16.0.64", equal, convert<string>(*uri.getIPv4()));
    ASSERT(55555, equal, uri.getPort());

    uri = convert<URI>(string("http://hostname.ru"));
    ASSERT(!uri.getHost().empty());
    ASSERT("hostname.ru", equal, uri.getHost());

    auto path = URI("https://www.bitmex.com/api/v1").getPath();
    ASSERT(!path.empty());
    ASSERT("/api/v1", equal, path);

    // test dns resolver
//    auto ya_ru_ipv4             = URI("http://ya.ru").getIPv4();
//    auto ya_ru_ipv4_expected    = URI("http://77.88.55.242").getIPv4();

    auto ya_ru_ipv4 = URI("http://example.com").getIPv4();
    auto ya_ru_ipv4_expected = URI("http://93.184.216.34").getIPv4();

    ASSERT(static_cast<bool>(ya_ru_ipv4));
    ASSERT(static_cast<bool>(ya_ru_ipv4_expected));
    ASSERT(*ya_ru_ipv4_expected, equal, *ya_ru_ipv4);
    ASSERT(URI("http://ya.rur").getIPv4(), std::exception);

    string value =
        "/1/2/Debug/iridium_test "
        "run --mode=raw --print-result=json /parsing/node.cpp";
    LOGT << URI(value).getHost();
}


} // io
} // iridium
