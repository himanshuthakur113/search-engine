#include <gtest/gtest.h>
#include "visited_set.h"


TEST(VisitedSetTest, InitiallyEmpty)
{
    VisitedSet visited;

    EXPECT_FALSE(visited.contains("https://example.com"));
}

// Mark one URL as visited

TEST(VisitedSetTest, MarkVisited)
{
    VisitedSet visited;

    visited.markVisited("https://example.com");

    EXPECT_TRUE(visited.contains("https://example.com"));
}

// Multiple URLs

TEST(VisitedSetTest, MultipleURLs)
{
    VisitedSet visited;

    visited.markVisited("https://a.com");
    visited.markVisited("https://b.com");
    visited.markVisited("https://c.com");

    EXPECT_TRUE(visited.contains("https://a.com"));
    EXPECT_TRUE(visited.contains("https://b.com"));
    EXPECT_TRUE(visited.contains("https://c.com"));
}

// Duplicate insert

TEST(VisitedSetTest, DuplicateInsert)
{
    VisitedSet visited;

    visited.markVisited("https://example.com");
    visited.markVisited("https://example.com");

    EXPECT_TRUE(visited.contains("https://example.com"));
}

// Unknown URL

TEST(VisitedSetTest, UnknownURL)
{
    VisitedSet visited;

    visited.markVisited("https://example.com");

    EXPECT_FALSE(visited.contains("https://unknown.com"));
}