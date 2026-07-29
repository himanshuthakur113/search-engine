// test/test_html_parser.cpp

#include <gtest/gtest.h>
#include "Indexer/html_parser.h"

class HTMLParserTest : public ::testing::Test
{
protected:
    HTMLParser parser;
};

// --------------------------------------------------
// Empty HTML
// --------------------------------------------------

TEST_F(HTMLParserTest, EmptyHTML)
{
    EXPECT_EQ(parser.extract_text(""), "");
}

// --------------------------------------------------
// Plain text
// --------------------------------------------------

TEST_F(HTMLParserTest, PlainText)
{
    EXPECT_EQ(parser.extract_text("Hello World"),
              "Hello World");
}

// --------------------------------------------------
// Remove simple tags
// --------------------------------------------------

TEST_F(HTMLParserTest, RemoveSimpleTags)
{
    std::string html =
        "<html><body>Hello World</body></html>";

    EXPECT_EQ(parser.extract_text(html),
              "Hello World");
}

// --------------------------------------------------
// Nested tags
// --------------------------------------------------

TEST_F(HTMLParserTest, NestedTags)
{
    std::string html =
        "<div><h1>Hello</h1><p>World</p></div>";

    EXPECT_EQ(parser.extract_text(html),
              "Hello World");
}

// --------------------------------------------------
// Ignore script content
// --------------------------------------------------

TEST_F(HTMLParserTest, IgnoreScript)
{
    std::string html =
        "<html>"
        "<script>"
        "int x = 10;"
        "</script>"
        "<body>Hello</body>"
        "</html>";

std::string result = parser.extract_text(html);

std::cout << result << std::endl;

EXPECT_EQ(result, "Hello");
}

// --------------------------------------------------
// Ignore style content
// --------------------------------------------------

TEST_F(HTMLParserTest, IgnoreStyle)
{
    std::string html =
        "<style>"
        "body{color:red;}"
        "</style>"
        "<p>Hello</p>";
std::string result = parser.extract_text(html);

std::cout << result << std::endl;

EXPECT_EQ(result, "Hello");
}

// --------------------------------------------------
// Ignore comments
// --------------------------------------------------

TEST_F(HTMLParserTest, IgnoreComments)
{
    std::string html =
        "Hello<!-- hidden -->World";

    EXPECT_EQ(parser.extract_text(html),
              "HelloWorld");
}

// --------------------------------------------------
// Multiple tags
// --------------------------------------------------

TEST_F(HTMLParserTest, MultipleTags)
{
    std::string html =
        "<h1>Crawler</h1>"
        "<p>Indexer</p>"
        "<p>Tokenizer</p>";

    EXPECT_EQ(parser.extract_text(html),
              "Crawler Indexer Tokenizer");
}

// --------------------------------------------------
// Mixed HTML
// --------------------------------------------------

TEST_F(HTMLParserTest, MixedHTML)
{
    std::string html =
        "<html>"
        "<head>"
        "<title>Demo</title>"
        "<style>.a{}</style>"
        "</head>"
        "<body>"
        "<h1>Hello</h1>"
        "<script>var x=5;</script>"
        "<p>World</p>"
        "</body>"
        "</html>";

    EXPECT_EQ(parser.extract_text(html),
              "Demo Hello World");
}

// --------------------------------------------------
// HTML with attributes
// --------------------------------------------------

TEST_F(HTMLParserTest, TagsWithAttributes)
{
    std::string html =
        "<a href=\"abc\">Click Here</a>";

    EXPECT_EQ(parser.extract_text(html),
              "Click Here");
}