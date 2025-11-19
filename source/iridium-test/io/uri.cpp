#include <iridium/testing/tester.h>
#include <iridium/io/uri.h>


using std::string;

#include <vector>
namespace iridium::io {


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

//    auto ipv4 = URI("http://example.org").getIPv4();
//    auto ipv4_expected = URI("http://23.215.0.132").getIPv4();

    auto ipv4 = URI("http://localhost").getIPv4();
    auto ipv4_expected = URI("http://127.0.0.1").getIPv4();

    ASSERT(static_cast<bool>(ipv4));
    ASSERT(static_cast<bool>(ipv4_expected));
    ASSERT(*ipv4_expected, equal, *ipv4);
    ASSERT(URI("http://ya.rur").getIPv4(), std::exception);

    {
        string source =
            "process:///1/2/Debug/iridium_test "
            "run --mode=raw --print-result=json /parsing/node.cpp";
        URI uri(source);

        ASSERT("/1/2/Debug/iridium_test", equal, uri.getAddress());
        ASSERT("iridium_test", equal, uri.getHost());
        ASSERT("run --mode=raw --print-result=json /parsing/node.cpp", equal, uri.getArguments());
    }
    ASSERT(URI("postgres://user:").getHost(), std::exception);
}


} // namespace iridium::io
