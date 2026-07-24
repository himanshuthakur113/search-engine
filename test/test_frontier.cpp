#include <gtest/gtest.h>
#include "frontier.h"
#include "crawl_task.h"

TEST(FrontierTest, InitiallyEmpty)
{
    Frontier frontier;

    EXPECT_TRUE(frontier.empty());
}

TEST(FrontierTest, PushOneTask)
{
    Frontier frontier;

    CrawlTask task("https://example.com", 0);

    frontier.push(task);

    EXPECT_FALSE(frontier.empty());
}

//fifo order

TEST(FrontierTest, PopReturnsFirstInsertedTask)
{
    Frontier frontier;

    frontier.push(CrawlTask("https://a.com", 0));
    frontier.push(CrawlTask("https://b.com", 1));

    CrawlTask task = frontier.pop();

    EXPECT_EQ(task.url, "https://a.com");
    EXPECT_EQ(task.depth, 0);
}



TEST(FrontierTest, MultiplePushAndPop)
{
    Frontier frontier;

    frontier.push(CrawlTask("https://a.com", 0));
    frontier.push(CrawlTask("https://b.com", 1));
    frontier.push(CrawlTask("https://c.com", 2));

    EXPECT_EQ(frontier.pop().url, "https://a.com");
    EXPECT_EQ(frontier.pop().url, "https://b.com");
    EXPECT_EQ(frontier.pop().url, "https://c.com");

    EXPECT_TRUE(frontier.empty());
}

TEST(FrontierTest, PushAfterEmpty)
{
    Frontier frontier;

    frontier.push(CrawlTask("https://a.com", 0));
    frontier.pop();

    EXPECT_TRUE(frontier.empty());

    frontier.push(CrawlTask("https://new.com", 5));

    CrawlTask task = frontier.pop();

    EXPECT_EQ(task.url, "https://new.com");
    EXPECT_EQ(task.depth, 5);
    EXPECT_TRUE(frontier.empty());
}