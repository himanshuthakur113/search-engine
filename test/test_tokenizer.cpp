#include <gtest/gtest.h>
#include "Indexer/tokenizer.h"

class TokenizerTest : public ::testing::Test
{
protected:
    Tokenizer tokenizer;
};


TEST_F(TokenizerTest, EmptyString)
{
    DA<std::string> tokens = tokenizer.tokenize("");

    EXPECT_EQ(tokens.size(), 0);
}


TEST_F(TokenizerTest, SingleWord)
{
    DA<std::string> tokens = tokenizer.tokenize("Hello");

    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0], "Hello");
}


TEST_F(TokenizerTest, MultipleWords)
{
    DA<std::string> tokens =
        tokenizer.tokenize("Hello World Indexer");

    ASSERT_EQ(tokens.size(), 3);

    EXPECT_EQ(tokens[0], "Hello");
    EXPECT_EQ(tokens[1], "World");
    EXPECT_EQ(tokens[2], "Indexer");
}

TEST_F(TokenizerTest, MultipleSpaces)
{
    DA<std::string> tokens =
        tokenizer.tokenize("Hello     World");

    ASSERT_EQ(tokens.size(), 2);

    EXPECT_EQ(tokens[0], "Hello");
    EXPECT_EQ(tokens[1], "World");
}


TEST_F(TokenizerTest, NewLines)
{
    DA<std::string> tokens =
        tokenizer.tokenize("Hello\nWorld\nIndexer");

    ASSERT_EQ(tokens.size(), 3);

    EXPECT_EQ(tokens[0], "Hello");
    EXPECT_EQ(tokens[1], "World");
    EXPECT_EQ(tokens[2], "Indexer");
}

// --------------------------------------------------
// Tabs
// --------------------------------------------------

TEST_F(TokenizerTest, Tabs)
{
    DA<std::string> tokens =
        tokenizer.tokenize("Hello\tWorld");

    ASSERT_EQ(tokens.size(), 2);

    EXPECT_EQ(tokens[0], "Hello");
    EXPECT_EQ(tokens[1], "World");
}

// --------------------------------------------------
// Punctuation
// --------------------------------------------------

TEST_F(TokenizerTest, Punctuation)
{
    DA<std::string> tokens =
        tokenizer.tokenize("Hello, World! Crawler.");

    ASSERT_EQ(tokens.size(), 3);

    EXPECT_EQ(tokens[0], "Hello");
    EXPECT_EQ(tokens[1], "World");
    EXPECT_EQ(tokens[2], "Crawler");
}

// --------------------------------------------------
// Numbers
// --------------------------------------------------

TEST_F(TokenizerTest, Numbers)
{
    DA<std::string> tokens =
        tokenizer.tokenize("123 456 789");

    ASSERT_EQ(tokens.size(), 3);

    EXPECT_EQ(tokens[0], "123");
    EXPECT_EQ(tokens[1], "456");
    EXPECT_EQ(tokens[2], "789");
}


TEST_F(TokenizerTest, AlphaNumeric)
{
    DA<std::string> tokens =
        tokenizer.tokenize("HTML5 GPT4 C++");

    ASSERT_EQ(tokens.size(), 3);
    std::cout << "Size = " << tokens.size() << '\n';

for(int i = 0; i < tokens.size(); ++i)
{
    std::cout << "[" << tokens[i] << "]\n";
}

    EXPECT_EQ(tokens[0], "HTML5");
    EXPECT_EQ(tokens[1], "GPT4");
    EXPECT_EQ(tokens[2], "C");
    
}


TEST_F(TokenizerTest, LeadingTrailingSpaces)
{
    DA<std::string> tokens =
        tokenizer.tokenize("   Hello World   ");

    ASSERT_EQ(tokens.size(), 2);

    EXPECT_EQ(tokens[0], "Hello");
    EXPECT_EQ(tokens[1], "World");
}