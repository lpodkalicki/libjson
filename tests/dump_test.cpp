/**
 * Copyright (c) 2023 by Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 */

#include <iostream>
#include <gtest/gtest.h>
#include "json.h"

using namespace testing;

TEST(DumpTest, checkBasicTypesDumpsCorrectly)
{
    EXPECT_EQ(json::Object(true).dumps(), "true");
    EXPECT_EQ(json::Object(123).dumps(), "123");
    EXPECT_EQ(json::Object(10.12).dumps(), "10.12");
    EXPECT_EQ(json::Object("text").dumps(), "\"text\"");
}

TEST(DumpTest, checkArrayDumpsCorrectly)
{
    EXPECT_EQ(json::Object({123}).dumps(false), "[123]");
    EXPECT_EQ(json::Object({true, 1, -2.01}).dumps(false), "[true,1,-2.01]");
    EXPECT_EQ(json::Object({"abc", "test", "hello"}).dumps(false), "[\"abc\",\"test\",\"hello\"]");
}

TEST(DumpTest, checkObjectDumpsCorrectly)
{
    EXPECT_EQ((json::Object{{"key"_, "value"}}.dumps(false)), "{\"key\":\"value\"}");
    EXPECT_EQ((json::Object{{"key1"_, "value"}, {"key2"_, true}}.dumps(false)), "{\"key1\":\"value\",\"key2\":true}");
    EXPECT_EQ((json::Object{{"key"_, {1,2,3}}}.dumps(false)), "{\"key\":[1,2,3]}");
    EXPECT_EQ((json::Object{{"key1"_, {1, 2, {{"key2"_, {1, 2, 3}}}}}}).dumps(false), "{\"key1\":[1,2,{\"key2\":[1,2,3]}]}");
    EXPECT_EQ((json::Object{{"key1"_, {{"key2"_, {true, false, "abc"}}}}}).dumps(false), "{\"key1\":{\"key2\":[true,false,\"abc\"]}}");
}

TEST(DumpTest, checkBasicObjectsPrettyDumpsWorksFine)
{
    EXPECT_EQ((json::Object{{"key"_, "value"}}.dumps(true)), "{\n  \"key\": \"value\"\n}");
    EXPECT_EQ((json::Object{{"key1"_, "value"}, {"key2"_, true}}.dumps(true)), "{\n  \"key1\": \"value\",\n  \"key2\": true\n}");
    EXPECT_EQ((json::Object{{"key"_, {1,2,3}}}.dumps(true)), "{\n  \"key\": [1, 2, 3]\n}");
    EXPECT_EQ((json::Object{{"key1"_, {1, 2, {{"key2"_, {1, 2, 3}}}}}, {"key3"_, true}}).dumps(true), "{\n  \"key1\": [1, 2, {\n    \"key2\": [1, 2, 3]\n  }],\n  \"key3\": true\n}");
    EXPECT_EQ((json::Object{{"key1"_, {{"key2"_, {true, false, "abc"}}, {"key3"_, 1}}}}).dumps(true), "{\n  \"key1\": {\n    \"key2\": [true, false, \"abc\"],\n    \"key3\": 1\n  }\n}");
}

TEST(DumpTest, checkAdvancedObjectPrettyDumpsWorksFine)
{
    json::Object obj{
        {"key1"_, "value"},
        {"key2"_, {{
            "key3"_, {{
                "key4"_, {{
                    "key5"_, "hello!"
                }}
            }}
        }}},
        {"key6"_, {1, 2, 3, {{
            "key7"_, {
                {"key8"_, {1, true, 3.0, "abc"}},
                {"key9"_, "test"},
                {"key10"_, true},
            }
        }}}}
    };

    std::string expected = "{\n  \"key1\": \"value\",\n  \"key2\": {\n    \"key3\": {\n      \"key4\": {\n"
                           "        \"key5\": \"hello!\"\n      }\n    }\n  },\n  \"key6\": [1, 2, 3, {\n"
                           "    \"key7\": {\n      \"key10\": true,\n      \"key8\": [1, true, 3, \"abc\"],\n"
                           "      \"key9\": \"test\"\n    }\n  }]\n}";

    EXPECT_EQ(obj.dumps(true), expected);
}
