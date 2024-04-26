#include <gtest/gtest.h>
#include "string_utils.h"

TEST(StringUtilsTest, TestSplit_ShortSentence) {
    std::string str = "hello world";
    std::string delimiter = " ";
    std::vector<std::string> result = utils::split(str, delimiter);
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "hello");
    EXPECT_EQ(result[1], "world");
}

TEST(StringUtilsTest, TestSplit_LongSentence) {
    std::string str = "hello world, this is a long sentence";
    std::string delimiter = " ";
    std::vector<std::string> result = utils::split(str, delimiter);
    EXPECT_EQ(result.size(), 7);
    EXPECT_EQ(result[0], "hello");
    EXPECT_EQ(result[1], "world,");
    EXPECT_EQ(result[2], "this");
    EXPECT_EQ(result[3], "is");
    EXPECT_EQ(result[4], "a");
    EXPECT_EQ(result[5], "long");
    EXPECT_EQ(result[6], "sentence");
}

TEST(StringUtilsTest, TestSplit_EmptyString) {
    std::string str = "";
    std::string delimiter = " ";
    std::vector<std::string> result = utils::split(str, delimiter);
    EXPECT_EQ(result.size(), 0);
}

TEST(StringUtilsTest, TestTrim) {
    std::string str = "  hello world  ";
    std::string result = utils::trim(str);
    EXPECT_EQ(result, "hello world");
}