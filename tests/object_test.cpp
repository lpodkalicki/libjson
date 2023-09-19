/**
 * Copyright (c) 2023 by Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 */

#include <iostream>
#include <gtest/gtest.h>
#include "json.h"

using namespace testing;

TEST(ObjectTest, checkObjectAssignmentConvertsTypesCorrectly)
{
    json::Object obj;

    EXPECT_TRUE(obj.isNull());
    EXPECT_EQ(obj.type(), json::Type::Null);

    obj = true;
    EXPECT_EQ(obj.type(), json::Type::Boolean);

    obj = 1;
    EXPECT_EQ(obj.type(), json::Type::Integer);

    obj = 1.0;
    EXPECT_EQ(obj.type(), json::Type::Double);

    obj = "abc";
    EXPECT_EQ(obj.type(), json::Type::String);
}


TEST(ObjectTest, AnotherCheck)
{
    json::Object obj{1, true, "string", 1.};

    EXPECT_FALSE(obj.isNull());
    EXPECT_EQ(obj.type(), json::Type::Array);

    obj = true;
    EXPECT_EQ(obj.type(), json::Type::Boolean);

    obj = 1;
    EXPECT_EQ(obj.type(), json::Type::Integer);

    obj = 1.0;
    EXPECT_EQ(obj.type(), json::Type::Double);

    obj = "abc";
    EXPECT_EQ(obj.type(), json::Type::String);
}

TEST(ObjectTest, checkArrayAssignmentWorksCorrectly)
{
    json::Object obj;

    obj["abc"] = {15, true, "example", 3.14};

    EXPECT_FALSE(obj.isNull());
    EXPECT_EQ(obj.type(), json::Type::Object);
    EXPECT_EQ(obj["abc"].type(), json::Type::Array);
    EXPECT_EQ(obj["abc"][0].type(), json::Type::Integer);
    EXPECT_EQ(obj["abc"][0].toInteger(), 15);
    EXPECT_EQ(obj["abc"][1].type(), json::Type::Boolean);
    EXPECT_EQ(obj["abc"][1].toBoolean(), true);
    EXPECT_EQ(obj["abc"][2].type(), json::Type::String);
    EXPECT_EQ(obj["abc"][2].toString(), "example");
    EXPECT_EQ(obj["abc"][3].type(), json::Type::Double);
    EXPECT_EQ(obj["abc"][3].toDouble(), 3.14);
}

TEST(ObjectTest, checkComplexObjectAssignmentWorksCorrectly)
{
    json::Object obj{
            {"key1"_, 1},
            {"key2"_, 2.0},
            {"key3"_, {1, 2, 3, 4, 5, "abc"}},
            {"key4"_, {
                {"key5"_, "jest ok!"},
                {"key6"_, 3},
                {"key7"_, {
                    {"key9"_, {4, 7, 9}}
                }},
            }},
            {"key10"_, false}
    };

    EXPECT_FALSE(obj.isNull());
    EXPECT_EQ(obj.type(), json::Type::Object);
    EXPECT_EQ(obj["key1"].type(), json::Type::Integer);
}