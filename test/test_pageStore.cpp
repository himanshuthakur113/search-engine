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

    EXPECT_EQ(store.pages_saved(), 1);

    store.flush();

    EXPECT_EQ(store.pages_saved(), 0);
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

    EXPECT_EQ(store.pages_saved(), 0);
}