#include <gtest/gtest.h>
#include "Indexer/posting_list.h"

class PostingListTest : public ::testing::Test
{
protected:
    PostingList postingList;
};

TEST_F(PostingListTest, InitiallyEmpty)
{
    EXPECT_EQ(postingList.getPostings().size(), 0);
}

// --------------------------------------------------
// Add Single Posting
// --------------------------------------------------

TEST_F(PostingListTest, AddSinglePosting)
{
    postingList.add_posting(10);

    const DA<Posting>& postings = postingList.getPostings();

    ASSERT_EQ(postings.size(), 1);
    EXPECT_EQ(postings[0].page_id, 10);
}


TEST_F(PostingListTest, AddMultiplePostings)
{
    postingList.add_posting(1);
    postingList.add_posting(5);
    postingList.add_posting(10);

    const DA<Posting>& postings = postingList.getPostings();

    ASSERT_EQ(postings.size(), 3);

    EXPECT_EQ(postings[0].page_id, 1);
    EXPECT_EQ(postings[1].page_id, 5);
    EXPECT_EQ(postings[2].page_id, 10);
}

TEST_F(PostingListTest, ContainsExistingPage)
{
    postingList.add_posting(25);

    EXPECT_TRUE(postingList.contains(25));
}


TEST_F(PostingListTest, ContainsMissingPage)
{
    postingList.add_posting(25);

    EXPECT_FALSE(postingList.contains(50));
}


TEST_F(PostingListTest, DuplicatePosting)
{
    postingList.add_posting(7);
    postingList.add_posting(7);

    const DA<Posting>& postings = postingList.getPostings();

    ASSERT_EQ(postings.size(), 1);
    EXPECT_TRUE(postingList.contains(7));
}

TEST_F(PostingListTest, LargeNumberOfPostings)
{
    for(int i = 0; i < 100; ++i)
    {
        postingList.add_posting(i);
    }

    EXPECT_EQ(postingList.getPostings().size(), 100);

    for(int i = 0; i < 100; ++i)
    {
        EXPECT_TRUE(postingList.contains(i));
    }
}