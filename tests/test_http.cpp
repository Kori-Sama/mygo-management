#include <gtest/gtest.h>
#include "protocol.h"
using namespace http;

TEST(HttpTest, TestRequestParser_ReturnAndNewline) {
    HttpRequest request = HttpRequest::parse("GET / HTTP/1.1\r\nHost: localhost\r\n\r\n");
    EXPECT_EQ(request.request_line, "GET / HTTP/1.1");
    EXPECT_EQ(request.header_kv["Host"], "localhost");
}

TEST(HttpTest, TestRequestParser_OnlyNewline) {
    HttpRequest request = HttpRequest::parse("GET / HTTP/1.1\nHost: localhost\n\n");
    EXPECT_EQ(request.request_line, "GET / HTTP/1.1");
    EXPECT_EQ(request.header_kv["Host"], "localhost");
}

TEST(HttpTest, TestRequestParser_WithBody) {
    HttpRequest request = HttpRequest::parse("POST / HTTP/1.1\nHost: localhost\nContent-Length: 5\n\nhello");
    EXPECT_EQ(request.request_line, "POST / HTTP/1.1");
    EXPECT_EQ(request.header_kv["Host"], "localhost");
    EXPECT_EQ(request.header_kv["Content-Length"], "5");
    EXPECT_EQ(request.body, "hello");
}