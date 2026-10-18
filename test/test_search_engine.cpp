#include <gtest/gtest.h>
#include <cmath>
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

// --------------------------------------------------
// Ranking (same three pages: ids 1, 2, 3)
// --------------------------------------------------

TEST_F(SearchEngineTest, PageWithMoreOccurrencesRanksFirst)
{
    InvertedIndex ranked;
    ranked.add_word("life", 1, 1);
    ranked.add_word("life", 2, 5);
    SearchEngine engine(ranked, *store);

    DA<SearchResult> results = engine.search("life");

    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].page_id, 2);
    EXPECT_EQ(results[1].page_id, 1);
    EXPECT_GT(results[0].score, results[1].score);
}

TEST_F(SearchEngineTest, ResultsAreSortedByScoreDescending)
{
    InvertedIndex ranked;
    ranked.add_word("life", 1, 2);
    ranked.add_word("life", 2, 9);
    ranked.add_word("life", 3, 4);
    SearchEngine engine(ranked, *store);

    DA<SearchResult> results = engine.search("life");

    ASSERT_EQ(results.size(), 3);
    EXPECT_EQ(results[0].page_id, 2);
    EXPECT_EQ(results[1].page_id, 3);
    EXPECT_EQ(results[2].page_id, 1);
}

TEST_F(SearchEngineTest, EqualScoresAreOrderedByPageId)
{
    InvertedIndex ranked;
    ranked.add_word("life", 3, 2);
    ranked.add_word("life", 1, 2);
    ranked.add_word("life", 2, 2);
    SearchEngine engine(ranked, *store);

    DA<SearchResult> results = engine.search("life");

    ASSERT_EQ(results.size(), 3);
    EXPECT_EQ(results[0].page_id, 1);
    EXPECT_EQ(results[1].page_id, 2);
    EXPECT_EQ(results[2].page_id, 3);
}

TEST_F(SearchEngineTest, ScoreIsCountTimesIdf)
{
    InvertedIndex ranked;
    ranked.add_word("life", 1, 3);
    ranked.add_word("life", 2, 1);
    SearchEngine engine(ranked, *store);

    DA<SearchResult> results = engine.search("life");

    // 3 pages in total, "life" is on 2 of them: idf = ln(1 + 3/2)
    double idf = std::log(1.0 + 3.0 / 2.0);
    ASSERT_EQ(results.size(), 2);
    EXPECT_NEAR(results[0].score, 3 * idf, 1e-9);
    EXPECT_NEAR(results[1].score, 1 * idf, 1e-9);
}

TEST_F(SearchEngineTest, ScoreOfSeveralTermsIsTheSum)
{
    InvertedIndex ranked;
    ranked.add_word("life", 2, 2);
    ranked.add_word("life", 1, 1);
    ranked.add_word("love", 2, 4);
    SearchEngine engine(ranked, *store);

    DA<SearchResult> results = engine.search("life love");

    double idf_life = std::log(1.0 + 3.0 / 2.0);
    double idf_love = std::log(1.0 + 3.0 / 1.0);
    ASSERT_EQ(results.size(), 1);
    EXPECT_NEAR(results[0].score, 2 * idf_life + 4 * idf_love, 1e-9);
}

// Both pages contain 5 query words in total, but "zebra" is rarer than
// "love", so the page that mentions "zebra" more often must win.
TEST_F(SearchEngineTest, RareWordsCountForMoreThanCommonWords)
{
    InvertedIndex ranked;
    ranked.add_word("love", 1, 4);
    ranked.add_word("love", 2, 1);
    ranked.add_word("love", 3, 1);   // "love" is on all 3 pages
    ranked.add_word("zebra", 1, 1);
    ranked.add_word("zebra", 2, 4);  // "zebra" is on only 2
    SearchEngine engine(ranked, *store);

    DA<SearchResult> results = engine.search("love zebra");

    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].page_id, 2);
    EXPECT_EQ(results[1].page_id, 1);
}

TEST_F(SearchEngineTest, WordOnEveryPageStillGetsPositiveScore)
{
    InvertedIndex ranked;
    ranked.add_word("life", 1, 1);
    ranked.add_word("life", 2, 1);
    ranked.add_word("life", 3, 1);
    SearchEngine engine(ranked, *store);

    DA<SearchResult> results = engine.search("life");

    ASSERT_EQ(results.size(), 3);
    EXPECT_GT(results[2].score, 0);
}

TEST(SearchRankingSortTest, ManyResultsComeBackInDescendingOrder)
{
    std::string dir = "search_ranking_sort_data";
    std::filesystem::remove_all(dir);

    {
        PageStore store(dir);
        InvertedIndex index;

        for(int i = 1; i <= 40; ++i)
        {
            Page page;
            page.url = "https://site.test/p" + std::to_string(i);
            page.html = "<html></html>";
            store.save(page);
            // counts jump around: 1..7 in a repeating pattern
            index.add_word("word", i, (i * 5) % 7 + 1);
        }
        store.flush();

        SearchEngine engine(index, store);
        DA<SearchResult> results = engine.search("word");

        ASSERT_EQ(results.size(), 40);
        for(int i = 1; i < results.size(); ++i)
        {
            bool in_order = results[i - 1].score > results[i].score ||
                            (results[i - 1].score == results[i].score &&
                             results[i - 1].page_id < results[i].page_id);
            EXPECT_TRUE(in_order) << "result " << i << " is out of order";
        }
    }

    std::filesystem::remove_all(dir);
}
