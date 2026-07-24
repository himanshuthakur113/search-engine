#include <gtest/gtest.h>
#include <string>

#include "fetcher.h"

//constructor for fetcher

TEST(FetcherTest, ConstructorDoesNotThrow)
{
    EXPECT_NO_THROW(Fetcher fetcher);
}


TEST(FetcherTest, FetchValidURL)
{
    Fetcher fetcher;

    std::string html = fetcher.fetch("https://example.com");

    EXPECT_FALSE(html.empty());
}

//the fetcher should return html page

TEST(FetcherTest, ReturnedContentContainsHTML)
{
    Fetcher fetcher;

    std::string html = fetcher.fetch("https://www.google.com");

    EXPECT_NE(html.find("<html>"), std::string::npos);
}

//test for invalid url

TEST(FetcherTest, InvalidURLThrows)
{
    Fetcher fetcher;

    EXPECT_THROW(
        fetcher.fetch("invalid-url"),
        std::runtime_error
    );
}

//test for invalid domain

TEST(FetcherTest, InvalidDomainThrows)
{
    Fetcher fetcher;

    EXPECT_THROW(
        fetcher.fetch("https://this-domain-should-not-exist-123456789.com"),
        std::runtime_error
    );
}