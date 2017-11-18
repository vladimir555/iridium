#include <gtest/gtest.h>


#include <iostream>
using namespace std;


#include "utility/networking/http/request.h"
#include "utility/networking/http/response.h"
#include "utility/parsing/implementation/parser_http.h"
#include "utility/parsing/implementation/parser_json.h"
#include "utility/parsing/implementation/parser_xml.h"
#include "utility/networking/server/implementation/http.h"
#include "utility/networking/server/implementation/http_fs_mapper.h"
#include "utility/logging/logger.h"


using utility::parsing::implementation::CHTTPParser;
using utility::parsing::implementation::CJSONParser;
using utility::parsing::implementation::CXMLParser;
using utility::convertion::convert;


#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


int create_socket(int port)
{
    int s;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
    perror("Unable to create socket");
    exit(EXIT_FAILURE);
    }

    if (::bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("Unable to bind");
    exit(EXIT_FAILURE);
    }

    if (listen(s, 1) < 0) {
    perror("Unable to listen");
    exit(EXIT_FAILURE);
    }

    return s;
}


void init_openssl()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}


void cleanup_openssl()
{
    EVP_cleanup();
}


SSL_CTX *create_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = SSLv23_server_method();

    ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

void configure_context(SSL_CTX *ctx)
{
    SSL_CTX_set_ecdh_auto(ctx, 1);

    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "utility.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file (ctx, "utility.key", SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
    }
}

int test()
{
    int sock;
    SSL_CTX *ctx;

    init_openssl();
    ctx = create_context();

    configure_context(ctx);

    sock = create_socket(4433);

    /* Handle connections */
    while(1) {
        struct sockaddr_in addr;
        uint len = sizeof(addr);
        SSL *ssl;
        const char reply[] = "test\n";

        int client = accept(sock, (struct sockaddr*)&addr, &len);
        if (client < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        }
        else {
            SSL_write(ssl, reply, strlen(reply));
        }

        SSL_free(ssl);
        close(client);
    }

    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
}

//TEST(networking, openssl) {
//    test();
//}


namespace utility {
namespace networking {
namespace http {


string const request_example = ""
"GET / HTTP/1.1\n"
"host: www.google.com\n"
"user-agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.9.2) Gecko/20100115 Firefox/3.6 GTB7.1\n"
"accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
"accept-Language: ru,en-us;q=0.7,en;q=0.3\n"
"accept-Encoding: gzip,deflate\n"
"accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7\n"
"keep-alive: 115\n"
"connection: keep-alive\n"
"cookie: PREF=ID=2578ccacb1ecf2aa:U=3a0a80ae418769c4:TM=1264579766:LM=1284111039:DV=sTIUytipGDoC:S=N0XC7wB0v7c6CCnH\n";


string const responce_example = ""
"HTTP/1.1 200 OK\n"
"date: Mon, 27 Jul 2009 12:28:53 GMT\n"
"server: Apache/2.2.14 (Win32)\n"
"last-modified: Wed, 22 Jul 2009 19:15:56 GMT\n"
"content-length: 88\n"
"content-type: text/html\n\n\n"
"<html>\n"
"<body>\n"
"<h1>Hello, World!::</h1>\n"
"</body>\n"
"</html>\n";


TEST(networking, http_request) {
    auto parser = CHTTPParser::create();
    auto node   = parser->parse(request_example);

    try {
//        cout << "node: " << convert<string>(node) << endl;

//        {
//            auto json = CJSONParser::create();
//            auto xml  = CXMLParser::create();
//            cout << "node: " << endl << convert<string>(node) << endl;
//            cout << "http: " << endl << parser->compose(node) << endl;
//            cout << "json: " << endl << json->compose(node) << endl;
//            cout << "xml : " << endl <<  xml->compose(node) << endl;
//        }


        request::THttp http(node);
//        ASSERT_EQ(request_example, parser->compose(http.getNode()));

//        cout << "http message method    : " << convert<string>(http.Message.get().method) << endl;
//        cout << "http message uri       : " << http.Message.get().uri << endl;
//        cout << "http message protocol  : " << http.Message.get().protocol << endl;

//        cout << "http header host       : " << http.Headers.Host.get() << endl;

//        for (auto const &accept: http.Headers.Accept)
//            cout << "accept: " << accept.get() << endl;

    } catch (std::exception const &e) {
        cout << e.what() << endl;
        FAIL();
    }
}


TEST(networking, http_response) {
    auto parser = CHTTPParser::create();
    auto node   = parser->parse(responce_example);

//    cout << "node:" << endl << convert<string>(node) << endl;

    response::THttp http(node);

    auto node_  = http.getNode();
    auto str    = parser->compose(node_);

//    cout << "response:" << endl << str << endl;

    ASSERT_EQ(responce_example, str);
}


//class HTTPHandler: public server::IHTTPHandler {
//public:
//    DEFINE_CREATE(HTTPHandler)
//    virtual ~HTTPHandler() = default;
//    ISocket::TPacket handle(TRequest const &request) override {
//        i++;
//        return  convert<ISocket::TPacket>
//            ("<html><body>Hello! uri = " + request.uri +
//            " " + convert<string>(i) + " " + "</body></html>");
//    }
//private:
//    int i = 0;
//};


TEST(networking, http_server) {
    logging::update(true);

    server::IHTTP::THTTPHandlers    handlers;
    for (int i = 0; i < 2; i++)
        handlers.push_back(server::implementation::CHTTPFSMapper::create("html"));
    server::IHTTP::TSharedPtr       http_server = server::implementation::CHTTP::create(URL("https://127.0.0.1:55555"), handlers);

    http_server->initialize();
    LOGT << "start !";

//    for (auto i = 0; i < 10; i++)
//        sleep(500);
    sleep(1);

    LOGT << "stop";
    http_server->finalize();
}


} // socket
} // networking
} // utility
