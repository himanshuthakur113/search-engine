#include <gtest/gtest.h>
#include "Indexer/inverted_index.h"

class InvertedIndexTest : public ::testing::Test
{
protected:
    InvertedIndex index;
};

TEST_F(InvertedIndexTest, InitiallyEmpty)
{
    EXPECT_FALSE(index.contains("crawler"));
    EXPECT_EQ(index.search("crawler"), nullptr);
    EXPECT_EQ(index.giveKey().size(), 0);
}

TEST_F(InvertedIndexTest, AddSingleWord)
{
    index.add_word("crawler", 1);

    EXPECT_TRUE(index.contains("crawler"));

    const PostingList* postingList = index.search("crawler");

    ASSERT_NE(postingList, nullptr);
    ASSERT_EQ(postingList->getPostings().size(), 1);
    EXPECT_EQ(postingList->getPostings()[0].page_id, 1);
}

TEST_F(InvertedIndexTest, SearchMissingWord)
{
    index.add_word("crawler", 1);

    EXPECT_FALSE(index.contains("indexer"));
    EXPECT_EQ(index.search("indexer"), nullptr);
}

TEST_F(InvertedIndexTest, MultiplePagesSameWord)
{
    index.add_word("crawler", 1);
    index.add_word("crawler", 5);
    index.add_word("crawler", 10);

    const PostingList* postingList = index.search("crawler");

    ASSERT_NE(postingList, nullptr);
    ASSERT_EQ(postingList->getPostings().size(), 3);

    EXPECT_EQ(postingList->getPostings()[0].page_id, 1);
    EXPECT_EQ(postingList->getPostings()[1].page_id, 5);
    EXPECT_EQ(postingList->getPostings()[2].page_id, 10);
}


TEST_F(InvertedIndexTest, MultipleKeywords)
{
    index.add_word("crawler", 1);
    index.add_word("indexer", 2);
    index.add_word("search", 3);

    EXPECT_TRUE(index.contains("crawler"));
    EXPECT_TRUE(index.contains("indexer"));
    EXPECT_TRUE(index.contains("search"));

    EXPECT_EQ(index.giveKey().size(), 3);
}

TEST_F(InvertedIndexTest, DuplicatePageId)
{
    index.add_word("crawler", 1);
    index.add_word("crawler", 1);

    const PostingList* postingList = index.search("crawler");

    ASSERT_NE(postingList, nullptr);

    // Change to 2 if PostingList allows duplicates
    EXPECT_EQ(postingList->getPostings().size(), 1);
}


TEST_F(InvertedIndexTest, GetKeys)
{
    index.add_word("crawler", 1);
    index.add_word("indexer", 2);
    index.add_word("search", 3);

    DA<std::string> keys = index.giveKey();

    ASSERT_EQ(keys.size(), 3);

    EXPECT_TRUE(index.contains(keys[0]));
    EXPECT_TRUE(index.contains(keys[1]));
    EXPECT_TRUE(index.contains(keys[2]));
}

TEST_F(InvertedIndexTest, LargeIndex)
{
    for(int i = 0; i < 100; ++i)
    {
        index.add_word("word" + std::to_string(i), i);
    }

    EXPECT_EQ(index.giveKey().size(), 100);

    for(int i = 0; i < 100; ++i)
    {
        EXPECT_TRUE(index.contains("word" + std::to_string(i)));
    }
}

// --------------------------------------------------
// Word counts
// --------------------------------------------------

TEST(InvertedIndexCountTest, RepeatedWordOnSamePageIsCounted)
{
    InvertedIndex index;

    index.add_word("crawler", 1);
    index.add_word("crawler", 1);
    index.add_word("crawler", 1);
    index.add_word("crawler", 2);

    const PostingList* postingList = index.search("crawler");
    ASSERT_NE(postingList, nullptr);

    EXPECT_EQ(postingList->getPostings().size(), 2);
    EXPECT_EQ(postingList->count_for(1), 3);
    EXPECT_EQ(postingList->count_for(2), 1);
}

TEST(InvertedIndexCountTest, CountCanBeGivenExplicitly)
{
    InvertedIndex index;

    index.add_word("search", 5, 4);

    EXPECT_EQ(index.search("search")->count_for(5), 4);
}
