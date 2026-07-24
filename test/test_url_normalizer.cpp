#include <gtest/gtest.h>
#include "url_handler.h"

TEST(URLHandler, NormalizeLowercase)
{
    URL url("https://example.com");

    EXPECT_EQ(
        url.normalize_url("HTTPS://EXAMPLE.COM/Index.html"),
        "https://example.com/Index.html"
    );
}

TEST(URLHandler, RemoveDefaultHTTPPort)
{
    URL url("http://example.com");

    EXPECT_EQ(
        url.normalize_url("http://example.com:80/test"),
        "http://example.com/test"
    );
}

TEST(URLHandler, RemoveDefaultHTTPSPort)
{
    URL url("https://example.com");

    EXPECT_EQ(
        url.normalize_url("https://example.com:443/test"),
        "https://example.com/test"
    );
}

TEST(URLHandler, KeepNonDefaultPort)
{
    URL url("https://example.com");

    EXPECT_EQ(
        url.normalize_url("https://example.com:8080/test"),
        "https://example.com:8080/test"
    );
}

TEST(URLHandler, RemoveFragment)
{
    URL url("https://example.com");

    EXPECT_EQ(
        url.normalize_url("https://example.com/index.html#section"),
        "https://example.com/index.html"
    );
}

TEST(URLHandler, KeepQuery)
{
    URL url("https://example.com");

    EXPECT_EQ(
        url.normalize_url("https://example.com/search?q=crawler"),
        "https://example.com/search?q=crawler"
    );
}

TEST(URLHandler, NormalizeDotSegments)
{
    URL url("https://example.com");

    std::string result = url.normalize_url("https://example.com/a/b/../c");

    std::cout << "Result = " << result << std::endl;

    EXPECT_EQ(result, "https://example.com/a/c");
}

TEST(URLHandler, NormalizeCurrentDirectory)
{
    URL url("https://example.com");

    EXPECT_EQ(
        url.normalize_url("https://example.com/a/./b"),
        "https://example.com/a/b"
    );
}

TEST(URLHandler, EmptyPathBecomesSlash)
{
    URL url("https://example.com");

    EXPECT_EQ(
        url.normalize_url("https://example.com"),
        "https://example.com/"
    );
}