/**
 * Copyright (c) 2023 by Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 */

#include <gtest/gtest.h>
#include "json.h"

using namespace testing;

TEST(ParserTest, checkThatParsingEmptyJsonStringGivesNullObject)
{
    json::Object obj = json::parse("");
    EXPECT_EQ(obj.type(), json::Type::Null);
}

TEST(ParserTest, checkThatParsingEmptyCurlyBracketsGivesEmptyJsonObject)
{
    std::list<std::string> samples{
        "{}",
        " {}",
        " {} \t",
        " {               } ",
        "\n\r\t\n    {\t\n\r\n   \t\n\r\n}    \n\t\t"
    };

    for (auto& sample: samples)
    {
        json::Object obj = json::parse(sample);
        EXPECT_EQ(obj.type(), json::Type::Object);
        EXPECT_TRUE(obj.isEmpty());
    }
}

TEST(ParserTest, checkThatParsingEmptySquareBracketsGivesEmptyJsonArray)
{
    std::list<std::string> samples{
        "[]",
        " []",
        " [] \t",
        " [               ] ",
        "\n\r\t\n    [\t\n\r\n   \t\n\r\n]    \n\t\t"
    };

    for (auto& sample: samples)
    {
        json::Object obj = json::parse(sample);
        EXPECT_EQ(obj.type(), json::Type::Array);
        EXPECT_TRUE(obj.isEmpty());
    }
}

TEST(ParserTest, checkParserHandlesBooleanTypesCorrectly)
{
    EXPECT_EQ(json::parse("true").type(), json::Type::Boolean);
    EXPECT_EQ(json::parse("false").type(), json::Type::Boolean);
}

TEST(ParserTest, verifyParsingOfBooleanValueTrueIsOk)
{
    std::list<std::string> samples{
            "true",
            " true",
            " true \t",
            " true ",
            "\n\r\t\n    true    \n\t\t"
    };

    for (auto& sample: samples)
    {
        json::Object obj = json::parse(sample);
        EXPECT_EQ(obj.type(), json::Type::Boolean);
        EXPECT_TRUE(obj.toBoolean());
    }
}

TEST(ParserTest, verifyParsingOfBooleanValueFalseIsOk)
{
    std::list<std::string> samples{
            "false",
            " false",
            " false \t",
            " false ",
            "\n\r\t\n    false    \n\t\t"
    };

    for (auto& sample: samples)
    {
        json::Object obj = json::parse(sample);
        EXPECT_EQ(obj.type(), json::Type::Boolean);
        EXPECT_FALSE(obj.toBoolean());
    }
}

TEST(ParserTest, checkParserHandlesIntegersCorrectly)
{
    EXPECT_EQ(json::parse("1000").type(), json::Type::Integer);
    EXPECT_EQ(json::parse(" 1023").toInteger(), 1023);
    EXPECT_EQ(json::parse(" 200\n").toInteger(), 200);
    EXPECT_EQ(json::parse(" \r0\r").toInteger(), 0);
    EXPECT_EQ(json::parse(" \t\t-10   \n").toInteger(), -10);
    EXPECT_THROW(json::parse("abc").toInteger(), json::Error);
    EXPECT_THROW(json::parse("12").toDouble(), json::Error);
}

TEST(ParserTest, checkParserHandlesDoublesCorrectly)
{
    EXPECT_EQ(json::parse("1000.").type(), json::Type::Double);
    EXPECT_EQ(json::parse(" 1023.").toDouble(), 1023.);
    EXPECT_EQ(json::parse(" 123.123\n").toDouble(), 123.123);
    EXPECT_EQ(json::parse(" \r10e3\n").toDouble(), 10e3);
    EXPECT_EQ(json::parse(" \t0.").toDouble(), 0.);
    EXPECT_EQ(json::parse("      -10.12\r").toDouble(), -10.12);
    EXPECT_EQ(json::parse(" \r-1000000.00001\n").toDouble(), -1000000.00001);
    EXPECT_EQ(json::parse(" \r-10.12e3\n").toDouble(), -10.12e3);
    EXPECT_THROW(json::parse(" \re12\n").toDouble(), json::Error);
    EXPECT_THROW(json::parse(" \r-1000000.00001\n").toInteger(), json::Error);
}

TEST(ParserTest, checkParserHandlesStringsCorrectly)
{
    EXPECT_EQ(json::parse("\"text\"").type(), json::Type::String);
    EXPECT_EQ(json::parse(" \"abc abc abc\n\r\t\" \r\r\r\n\t     ").toString(), "abc abc abc\n\r\t");
    EXPECT_THROW(json::parse(" 12     ").toString(), json::Error);
    EXPECT_THROW(json::parse("\"abc\"").toInteger(), json::Error);
}