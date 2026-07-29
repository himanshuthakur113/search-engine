#include <gtest/gtest.h>
#include "Indexer/normalizer.h"

class NormalizerTest : public ::testing::Test
{
protected:
    Normalizer normalizer;
};


TEST_F(NormalizerTest, Lowercase)
{
    EXPECT_EQ(normalizer.normalize("HELLO"), "hello");
}

TEST_F(NormalizerTest, MixedCase)
{
    EXPECT_EQ(normalizer.normalize("HeLLo"), "hello");
}

TEST_F(NormalizerTest, RemoveTrailingPunctuation)
{
    EXPECT_EQ(normalizer.normalize("hello."), "hello");
    EXPECT_EQ(normalizer.normalize("world,"), "world");
    EXPECT_EQ(normalizer.normalize("test!"), "test");
    EXPECT_EQ(normalizer.normalize("index?"), "index");
}


TEST_F(NormalizerTest, RemoveStopWord)
{
    EXPECT_EQ(normalizer.normalize("the"), "");
    EXPECT_EQ(normalizer.normalize("is"), "");
    EXPECT_EQ(normalizer.normalize("and"), "");
}


TEST_F(NormalizerTest, RemoveNumbers)
{
    EXPECT_EQ(normalizer.normalize("12345"), "");
    EXPECT_EQ(normalizer.normalize("2026"), "");
}


TEST_F(NormalizerTest, RemoveSingleCharacter)
{
    EXPECT_EQ(normalizer.normalize("a"), "");
    EXPECT_EQ(normalizer.normalize("x"), "");
}

TEST_F(NormalizerTest, ValidWord)
{
    EXPECT_EQ(normalizer.normalize("Crawler"), "crawler");
}



TEST_F(NormalizerTest, EmptyString)
{
    EXPECT_EQ(normalizer.normalize(""), "");
}



TEST_F(NormalizerTest, WordWithPunctuation)
{
    EXPECT_EQ(normalizer.normalize("Indexer!!!"), "indexer");
}


TEST_F(NormalizerTest, AlphaNumeric)
{
    EXPECT_EQ(normalizer.normalize("HTML5"), "html5");
}