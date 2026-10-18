#include <gtest/gtest.h>
#include <filesystem>

#include "Indexer/index_store.h"
#include "Indexer/inverted_index.h"
#include "sqlite_db.h"

class IndexStoreTest : public ::testing::Test
{
protected:
    // IndexStore takes a directory; it creates <dir>/index.db inside it
    std::string dir = "index_store_test_data";

    void SetUp() override
    {
        std::filesystem::remove_all(dir);
    }

    void TearDown() override
    {
        std::filesystem::remove_all(dir);
    }
};

// --------------------------------------------------
// Empty Index
// --------------------------------------------------

TEST_F(IndexStoreTest, SaveEmptyIndex)
{
    InvertedIndex index;
    IndexStore store(dir);

    EXPECT_NO_THROW(store.save(index));
}

// --------------------------------------------------
// Single Keyword
// --------------------------------------------------

TEST_F(IndexStoreTest, SaveSingleKeyword)
{
    InvertedIndex index;

    index.add_word("crawler", 1);

    IndexStore store(dir);

    EXPECT_NO_THROW(store.save(index));
}

// --------------------------------------------------
// Multiple Keywords
// --------------------------------------------------

TEST_F(IndexStoreTest, SaveMultipleKeywords)
{
    InvertedIndex index;

    index.add_word("crawler",1);
    index.add_word("indexer",2);
    index.add_word("search",3);

    IndexStore store(dir);

    EXPECT_NO_THROW(store.save(index));
}

// --------------------------------------------------
// Multiple Page IDs
// --------------------------------------------------

TEST_F(IndexStoreTest, SaveMultiplePostings)
{
    InvertedIndex index;

    index.add_word("crawler",1);
    index.add_word("crawler",2);
    index.add_word("crawler",3);

    IndexStore store(dir);

    EXPECT_NO_THROW(store.save(index));
}

// --------------------------------------------------
// Large Index
// --------------------------------------------------

TEST_F(IndexStoreTest, SaveLargeIndex)
{
    InvertedIndex index;

    for(int i=0;i<100;i++)
    {
        index.add_word("word"+std::to_string(i),i);
    }

    IndexStore store(dir);

    EXPECT_NO_THROW(store.save(index));
}

// --------------------------------------------------
// Loading the index back
// --------------------------------------------------

TEST_F(IndexStoreTest, LoadFromEmptyDatabaseGivesEmptyIndex)
{
    IndexStore store(dir);

    InvertedIndex loaded = store.load();

    EXPECT_EQ(loaded.giveKey().size(), 0);
}

TEST_F(IndexStoreTest, SavedIndexCanBeLoadedBack)
{
    InvertedIndex index;
    index.add_word("life", 1);
    index.add_word("life", 2);
    index.add_word("love", 2);

    IndexStore store(dir);
    store.save(index);

    InvertedIndex loaded = store.load();

    EXPECT_EQ(loaded.giveKey().size(), 2);
    ASSERT_TRUE(loaded.contains("life"));
    ASSERT_TRUE(loaded.contains("love"));
    EXPECT_EQ(loaded.search("life")->getPostings().size(), 2);
    EXPECT_TRUE(loaded.search("life")->contains(1));
    EXPECT_TRUE(loaded.search("life")->contains(2));
    EXPECT_EQ(loaded.search("love")->getPostings().size(), 1);
    EXPECT_TRUE(loaded.search("love")->contains(2));
}

TEST_F(IndexStoreTest, LoadSurvivesReopeningTheDatabase)
{
    {
        InvertedIndex index;
        index.add_word("quotes", 7);
        IndexStore store(dir);
        store.save(index);
    }

    IndexStore reopened(dir);
    InvertedIndex loaded = reopened.load();

    ASSERT_TRUE(loaded.contains("quotes"));
    EXPECT_TRUE(loaded.search("quotes")->contains(7));
}

TEST_F(IndexStoreTest, SaveReplacesPreviousIndex)
{
    IndexStore store(dir);

    InvertedIndex first;
    first.add_word("old", 1);
    store.save(first);

    InvertedIndex second;
    second.add_word("new", 1);
    store.save(second);

    InvertedIndex loaded = store.load();

    EXPECT_FALSE(loaded.contains("old"));
    EXPECT_TRUE(loaded.contains("new"));
}
