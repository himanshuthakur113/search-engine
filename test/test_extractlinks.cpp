#include <gtest/gtest.h>

#include "crawler.h"

//extract single link
TEST(WebCrawlerTest, ExtractSingleLink)
{
    WebCrawler crawler;

    std::string html =
        "<html>"
        "<body>"
        "<a href=\"https://example.com\">Example</a>"
        "</body>"
        "</html>";

    DA<std::string> links = crawler.link_extractor(html);

    ASSERT_EQ(links.size(), 1);
    EXPECT_EQ(links[0], "https://example.com");
}

//extract multiple links

TEST(WebCrawlerTest, ExtractMultipleLinks)
{
    WebCrawler crawler;

    std::string html =
        "<a href=\"a.html\">A</a>"
        "<a href=\"b.html\">B</a>"
        "<a href=\"c.html\">C</a>";

    DA<std::string> links = crawler.link_extractor(html);

    ASSERT_EQ(links.size(), 3);

    EXPECT_EQ(links[0], "a.html");
    EXPECT_EQ(links[1], "b.html");
    EXPECT_EQ(links[2], "c.html");
}

//do not extract links which does not have <a or href

TEST(WebCrawlerTest, IgnoreTagsWithoutHref)
{
    WebCrawler crawler;

    std::string html =
        "<a>Home</a>"
        "<a class=\"btn\">Button</a>"
        "<div href=\"fake.html\"></div>";

    DA<std::string> links = crawler.link_extractor(html);

    EXPECT_EQ(links.size(), 0);
}

// Handle quoted and unquoted href values

TEST(WebCrawlerTest, ExtractQuotedAndUnquotedLinks)
{
    WebCrawler crawler;

    std::string html =
        "<a href=\"a.html\"></a>"
        "<a href='b.html'></a>"
        "<a href=c.html></a>";

    DA<std::string> links = crawler.link_extractor(html);

    ASSERT_EQ(links.size(), 3);

    EXPECT_EQ(links[0], "a.html");
    EXPECT_EQ(links[1], "b.html");
    EXPECT_EQ(links[2], "c.html");
}


TEST(WebCrawlerTest, IgnoreEmptyHref)
{
    WebCrawler crawler;

    std::string html =
        "<a href=\"\"></a>"
        "<a href=''></a>";

    DA<std::string> links = crawler.link_extractor(html);

    EXPECT_EQ(links.size(), 0);
}

TEST(WebCrawlerTest, HandlesUppercaseTagsAndAttributes)
{
    WebCrawler crawler;

    std::string html =
        "<A HREF=\"upper.html\">Upper</A>";

    DA<std::string> links = crawler.link_extractor(html);

    ASSERT_EQ(links.size(), 1);
    EXPECT_EQ(links[0], "upper.html");
}

TEST(WebCrawlerTest, HrefAfterOtherAttributes)
{
    WebCrawler crawler;

    std::string html =
        "<a class=\"btn\" id=\"1\" href=\"page.html\">Link</a>";

    DA<std::string> links = crawler.link_extractor(html);

    ASSERT_EQ(links.size(), 1);
    EXPECT_EQ(links[0], "page.html");
}


TEST(WebCrawlerTest, MixedAnchorTags)
{
    WebCrawler crawler;

    std::string html =
        "<a href=\"one.html\"></a>"
        "<a></a>"
        "<a class=\"x\"></a>"
        "<a href=\"two.html\"></a>";

    DA<std::string> links = crawler.link_extractor(html);

    ASSERT_EQ(links.size(), 2);

    EXPECT_EQ(links[0], "one.html");
    EXPECT_EQ(links[1], "two.html");
}