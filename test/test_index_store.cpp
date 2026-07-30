#include <gtest/gtest.h>
#include <cstdio>

#include "Indexer/index_store.h"
#include "Indexer/inverted_index.h"
#include "sqlite_db.h"

class IndexStoreTest : public ::testing::Test
{
protected:
    std::string dbName = "test_index.db";

    void SetUp() override
    {
        std::remove(dbName.c_str());
    }

    void TearDown() override
    {
        std::remove(dbName.c_str());
    }
};

// --------------------------------------------------
// Empty Index
// --------------------------------------------------

TEST_F(IndexStoreTest, SaveEmptyIndex)
{
    InvertedIndex index;
    IndexStore store(dbName);

    EXPECT_NO_THROW(store.save(index));
}

// --------------------------------------------------
// Single Keyword
// --------------------------------------------------

TEST_F(IndexStoreTest, SaveSingleKeyword)
{
    InvertedIndex index;

    index.add_word("crawler", 1);

    IndexStore store(dbName);

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

    IndexStore store(dbName);

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

    IndexStore store(dbName);

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

    IndexStore store(dbName);

    EXPECT_NO_THROW(store.save(index));
}