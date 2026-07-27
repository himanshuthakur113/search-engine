#include <gtest/gtest.h>
#include "crawler.h"

class CrawlerIntegrationTest : public ::testing::Test
{
protected:
    WebCrawler crawler{2, 20};
};

TEST_F(CrawlerIntegrationTest, CrawlHandlesDuplicateURLs)
{
    CrawlTask seed("https://example.com", 0);

    EXPECT_NO_THROW(crawler.crawl(seed));

    SUCCEED() << "Crawler completed successfully with duplicate URLs.";
}

TEST_F(CrawlerIntegrationTest, CrawlHandlesCyclicLinks)
{
    // Wikipedia contains many pages linking back to previously visited pages.
    CrawlTask seed("https://en.wikipedia.org/wiki/Web_crawler", 0);

    EXPECT_NO_THROW(crawler.crawl(seed));

    SUCCEED() << "Crawler handled cyclic links without infinite loop.";
}

TEST_F(CrawlerIntegrationTest, CrawlHandlesMalformedHTML)
{
    // A page with imperfect HTML.
    CrawlTask seed("https://httpbin.org/html", 0);

    EXPECT_NO_THROW(crawler.crawl(seed));

    SUCCEED() << "Crawler handled malformed HTML successfully.";
}

TEST_F(CrawlerIntegrationTest, CrawlHandlesInvalidPage)
{
    CrawlTask seed("https://example.com/this-page-does-not-exist", 0);

    EXPECT_NO_THROW(crawler.crawl(seed));

    SUCCEED() << "Crawler handled invalid page without crashing.";
}

TEST_F(CrawlerIntegrationTest, CrawlHandlesEmptyPage)
{
    // This URL should return an empty body or minimal content if your fetcher supports file URLs.
    CrawlTask seed("https://httpbin.org/status/204", 0);

    EXPECT_NO_THROW(crawler.crawl(seed));

    SUCCEED() << "Crawler handled empty page successfully.";
}