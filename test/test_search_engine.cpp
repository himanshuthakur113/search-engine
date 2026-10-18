#include <gtest/gtest.h>
#include <filesystem>

#include "Search/search_engine.h"

// Three pages are stored (SQLite gives them ids 1, 2, 3):
//   1 -> /quotes   words: life, quotes
//   2 -> /life     words: life, love
//   3 -> /love     words: love
class SearchEngineTest : public ::testing::Test
{
protected:
    std::string dir = "search_engine_test_data";
    PageStore* store = nullptr;
    InvertedIndex index;

    void SetUp() override
    {
        std::filesystem::remove_all(dir);
        store = new PageStore(dir);

        add_page("https://site.test/quotes");
        add_page("https://site.test/life");
        add_page("https://site.test/love");
        store->flush();

        index.add_word("life", 1);
        index.add_word("quotes", 1);
        index.add_word("life", 2);
        index.add_word("love", 2);
        index.add_word("love", 3);
    }

    void TearDown() override
    {
        delete store;
        std::filesystem::remove_all(dir);
    }

    void add_page(const std::string& url)
    {
        Page page;
        page.url = url;
        page.html = "<html></html>";
        store->save(page);
    }
};

TEST_F(SearchEngineTest, SingleWordFindsAllPagesWithIt)
{
    SearchEngine engine(index, *store);

    DA<int> pages = engine.match("life");

    ASSERT_EQ(pages.size(), 2);
    EXPECT_EQ(pages[0], 1);
    EXPECT_EQ(pages[1], 2);
}

TEST_F(SearchEngineTest, MultipleWordsRequireAllWords)
{
    SearchEngine engine(index, *store);

    DA<int> pages = engine.match("life love");

    ASSERT_EQ(pages.size(), 1);
    EXPECT_EQ(pages[0], 2);
}

TEST_F(SearchEngineTest, WordOrderDoesNotMatter)
{
    SearchEngine engine(index, *store);

    DA<int> pages = engine.match("love life");

    ASSERT_EQ(pages.size(), 1);
    EXPECT_EQ(pages[0], 2);
}

TEST_F(SearchEngineTest, WordsWithNoCommonPageGiveNoResults)
{
    SearchEngine engine(index, *store);

    EXPECT_EQ(engine.match("quotes love").size(), 0);
}

TEST_F(SearchEngineTest, UnknownWordGivesNoResults)
{
    SearchEngine engine(index, *store);

    EXPECT_EQ(engine.match("zebra").size(), 0);
}

TEST_F(SearchEngineTest, OneUnknownWordMakesWholeQueryFail)
{
    SearchEngine engine(index, *store);

    EXPECT_EQ(engine.match("life zebra").size(), 0);
}

TEST_F(SearchEngineTest, QueryIsCaseInsensitive)
{
    SearchEngine engine(index, *store);

    DA<int> pages = engine.match("LiFe");

    EXPECT_EQ(pages.size(), 2);
}

TEST_F(SearchEngineTest, PunctuationInQueryIsIgnored)
{
    SearchEngine engine(index, *store);

    EXPECT_EQ(engine.match("life!").size(), 2);
    EXPECT_EQ(engine.match("life, love?").size(), 1);
}

TEST_F(SearchEngineTest, EmptyQueryGivesNoResults)
{
    SearchEngine engine(index, *store);

    EXPECT_EQ(engine.match("").size(), 0);
    EXPECT_EQ(engine.match("   ").size(), 0);
}

TEST_F(SearchEngineTest, StopwordsAreIgnoredInQuery)
{
    SearchEngine engine(index, *store);

    // "the" is a stopword, so this is the same as searching "life"
    EXPECT_EQ(engine.match("the life").size(), 2);
}

TEST_F(SearchEngineTest, QueryOfOnlyStopwordsGivesNoResults)
{
    SearchEngine engine(index, *store);

    EXPECT_EQ(engine.match("the a an").size(), 0);
}

TEST_F(SearchEngineTest, RepeatedWordsAreCountedOnce)
{
    SearchEngine engine(index, *store);

    DA<std::string> terms = engine.parse_query("life LIFE life");

    ASSERT_EQ(terms.size(), 1);
    EXPECT_EQ(terms[0], "life");
}

TEST_F(SearchEngineTest, SearchReturnsUrlsOfMatchingPages)
{
    SearchEngine engine(index, *store);

    DA<SearchResult> results = engine.search("life");

    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].page_id, 1);
    EXPECT_EQ(results[0].url, "https://site.test/quotes");
    EXPECT_EQ(results[1].page_id, 2);
    EXPECT_EQ(results[1].url, "https://site.test/life");
}

TEST_F(SearchEngineTest, SearchWithNoMatchGivesEmptyResults)
{
    SearchEngine engine(index, *store);

    EXPECT_EQ(engine.search("zebra").size(), 0);
}
