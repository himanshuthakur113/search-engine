#include <gtest/gtest.h>
#include <filesystem>

#include "page_store.h"

class PageStoreTest : public ::testing::Test
{
protected:
    std::string dir = "data";

    void SetUp() override
    {
        std::filesystem::create_directory(dir);
        std::filesystem::remove(dir + "/pages.db");
    }

    void TearDown() override
    {
        std::filesystem::remove(dir + "/pages.db");
    }
};

TEST_F(PageStoreTest, SaveOnePage)
{
    PageStore store(dir);

    Page page;
    page.url = "https://example.com";
    page.html = "<html>Hello</html>";

    store.save(page);

    // save() only buffers the page; it is written to the database on flush()
    EXPECT_EQ(store.buffered_pages(), 1);
    EXPECT_EQ(store.pages_saved(), 0);

    store.flush();

    EXPECT_EQ(store.buffered_pages(), 0);
    EXPECT_EQ(store.pages_saved(), 1);
}

TEST_F(PageStoreTest, FlushStoresPage)
{
    PageStore store(dir);

    Page page;
    page.url = "https://example.com";
    page.html = "<html>Hello</html>";

    store.save(page);
    store.flush();

    EXPECT_TRUE(store.isCached(page.url));
}

TEST_F(PageStoreTest, FetchStoredPage)
{
    PageStore store(dir);

    Page page;
    page.url = "https://example.com";
    page.html = "<html>Testing</html>";

    store.save(page);
    store.flush();

    EXPECT_EQ(
        store.fetch(page.url),
        page.html
    );
}

TEST_F(PageStoreTest, FlushClearsBuffer)
{
    PageStore store(dir);

    Page page;
    page.url = "https://example.com";
    page.html = "abc";

    store.save(page);

    EXPECT_EQ(store.buffered_pages(), 1);

    store.flush();

    EXPECT_EQ(store.buffered_pages(), 0);
    EXPECT_EQ(store.pages_saved(), 1);
}

TEST_F(PageStoreTest, DuplicatePageIgnored)
{
    PageStore store(dir);

    Page page;
    page.url = "https://example.com";
    page.html = "<html>Hello</html>";

    store.save(page);
    store.flush();

    store.save(page);

    // the second save is ignored: nothing new is buffered or written
    EXPECT_EQ(store.buffered_pages(), 0);
    EXPECT_EQ(store.pages_saved(), 1);
}

TEST_F(PageStoreTest, UrlOfReturnsUrlOfStoredPage)
{
    PageStore store(dir);

    Page page;
    page.url = "https://example.com/a";
    page.html = "<html>A</html>";

    store.save(page);
    store.flush();

    // SQLite numbers the first stored page 1
    EXPECT_EQ(store.url_of(1), "https://example.com/a");
}

TEST_F(PageStoreTest, UrlOfUnknownIdThrows)
{
    PageStore store(dir);

    EXPECT_THROW(store.url_of(999), std::runtime_error);
}

TEST_F(PageStoreTest, TotalPagesCountsStoredPages)
{
    PageStore store(dir);
    EXPECT_EQ(store.total_pages(), 0);

    Page a;
    a.url = "https://example.com/a";
    a.html = "<html>A</html>";
    Page b;
    b.url = "https://example.com/b";
    b.html = "<html>B</html>";

    store.save(a);
    store.save(b);
    store.flush();

    EXPECT_EQ(store.total_pages(), 2);
}
