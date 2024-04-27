#include <gtest/gtest.h>
#include "protocol.h"
using namespace http;

TEST(HttpTest, TestRequestParser_ReturnAndNewline) {
    HttpRequest request;
    request.parse("GET / HTTP/1.1\r\nHost: localhost\r\n\r\n");
    EXPECT_EQ(request.request_line, "GET / HTTP/1.1");
    EXPECT_EQ(request.header_kv["Host"], "localhost");
}

TEST(HttpTest, TestRequestParser_OnlyNewline) {
    HttpRequest request;
    request.parse("GET / HTTP/1.1\nHost: localhost\n\n");
    EXPECT_EQ(request.request_line, "GET / HTTP/1.1");
    EXPECT_EQ(request.header_kv["Host"], "localhost");
}

TEST(HttpTest, TestRequestParser_WithBody) {
    HttpRequest request;
    request.parse("POST / HTTP/1.1\r\nHost: localhost\nContent-Length: 5\n\nhello");
    EXPECT_EQ(request.request_line, "POST / HTTP/1.1");
    EXPECT_EQ(request.header_kv["Host"], "localhost");
    EXPECT_EQ(request.header_kv["Content-Length"], "5");
    EXPECT_EQ(request.body, "hello");
}

TEST(HttpTest, TestResponseBuilder) {
    std::unordered_map<std::string, std::string> header;

    header["Content-Type"] = "text/plain";
    header["Content-Length"] = "5";
    HttpResponse response;
    response.status_code = HttpCode::OK;
    response.header_kv = header;
    response.body = "hello";
    response.build();
    EXPECT_EQ(response.status_line, "HTTP/1.1 200 OK\r\n");
    EXPECT_EQ(response.header, "Content-Length: 5\r\nContent-Type: text/plain\r\n");
    EXPECT_EQ(response.body, "hello");

    std::string response_str = response.to_string();
    std::string expected = "HTTP/1.1 200 OK\r\nContent-Length: 5\r\nContent-Type: text/plain\r\n\r\nhello";
    EXPECT_EQ(response_str, expected);
}