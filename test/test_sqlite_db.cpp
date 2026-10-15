#include <gtest/gtest.h>
#include <cstdio>
#include <string>

#include "sqlite_db.h"
#include "page.h"

class SQLiteDBTest : public ::testing::Test
{
protected:
    std::string db_name = "test_database.db";

    void SetUp() override
    {
        std::remove(db_name.c_str());
    }

    void TearDown() override
    {
        std::remove(db_name.c_str());
    }
};

// Constructor

TEST_F(SQLiteDBTest, DatabaseCreation)
{
    EXPECT_NO_THROW(SQLiteDB db(db_name));
}

// exists()

TEST_F(SQLiteDBTest, ExistsReturnsFalseInitially)
{
    SQLiteDB db(db_name);
    db.initialize_page_table();

    EXPECT_FALSE(db.exists("https://example.com"));
}

TEST_F(SQLiteDBTest, ExistsReturnsTrueAfterInsert)
{
    SQLiteDB db(db_name);
    db.initialize_page_table();

    Page page;
    page.url = "https://example.com";
    page.html = "<html>Hello</html>";

    db.insert(page);

    EXPECT_TRUE(db.exists(page.url));
}

// insert()

TEST_F(SQLiteDBTest, InsertSinglePage)
{
    SQLiteDB db(db_name);
    db.initialize_page_table();

    Page page;
    page.url = "https://example.com";
    page.html = "<html>Hello World</html>";

    EXPECT_NO_THROW(db.insert(page));
}

TEST_F(SQLiteDBTest, InsertMultiplePages)
{
    SQLiteDB db(db_name);
    db.initialize_page_table();

    Page p1;
    p1.url = "https://a.com";
    p1.html = "AAA";

    Page p2;
    p2.url = "https://b.com";
    p2.html = "BBB";

    Page p3;
    p3.url = "https://c.com";
    p3.html = "CCC";

    db.insert(p1);
    db.insert(p2);
    db.insert(p3);

    EXPECT_TRUE(db.exists(p1.url));
    EXPECT_TRUE(db.exists(p2.url));
    EXPECT_TRUE(db.exists(p3.url));
}

TEST_F(SQLiteDBTest, DuplicateURLThrowsException)
{
    SQLiteDB db(db_name);
    db.initialize_page_table();

    Page page;
    page.url = "https://example.com";
    page.html = "HTML";

    db.insert(page);

    EXPECT_THROW(
        db.insert(page),
        std::runtime_error
    );
}

// fetch()

TEST_F(SQLiteDBTest, FetchHTML)
{
    SQLiteDB db(db_name);
    db.initialize_page_table();

    Page page;
    page.url = "https://example.com";
    page.html = "<html>Fetched HTML</html>";

    db.insert(page);

    EXPECT_EQ(
        db.fetch(page.url),
        page.html
    );
}

TEST_F(SQLiteDBTest, FetchUnknownURLThrows)
{
    SQLiteDB db(db_name);
    db.initialize_page_table();

    EXPECT_THROW(
        db.fetch("https://unknown.com"),
        std::runtime_error
    );
}

// get()

TEST_F(SQLiteDBTest, GetStoredPage)
{
    SQLiteDB db(db_name);
    db.initialize_page_table();

    Page page;
    page.url = "https://example.com";
    page.html = "<html>Testing</html>";

    db.insert(page);

    Page result = db.get(1);

    EXPECT_EQ(result.id, 1);
    EXPECT_EQ(result.url, page.url);
    EXPECT_EQ(result.html, page.html);
}

TEST_F(SQLiteDBTest, GetInvalidIDThrows)
{
    SQLiteDB db(db_name);
    db.initialize_page_table();

    EXPECT_THROW(
        db.get(10),
        std::runtime_error
    );
}

// Edge Cases

TEST_F(SQLiteDBTest, InsertEmptyHTML)
{
    SQLiteDB db(db_name);
    db.initialize_page_table();

    Page page;
    page.url = "https://example.com";
    page.html = "";

    db.insert(page);

    EXPECT_EQ(
        db.fetch(page.url),
        ""
    );
}

TEST_F(SQLiteDBTest, InsertLargeHTML)
{
    SQLiteDB db(db_name);
    db.initialize_page_table();

    Page page;
    page.url = "https://large.com";
    page.html = std::string(100000, 'A');

    db.insert(page);

    EXPECT_EQ(
        db.fetch(page.url),
        page.html
    );
}

TEST_F(SQLiteDBTest, MultipleFetches)
{
    SQLiteDB db(db_name);
    db.initialize_page_table();

    Page p1;
    p1.url = "https://a.com";
    p1.html = "AAA";

    Page p2;
    p2.url = "https://b.com";
    p2.html = "BBB";

    db.insert(p1);
    db.insert(p2);

    EXPECT_EQ(db.fetch(p1.url), "AAA");
    EXPECT_EQ(db.fetch(p2.url), "BBB");
}

TEST_F(SQLiteDBTest, GetMultiplePages)
{
    SQLiteDB db(db_name);
    db.initialize_page_table();

    Page p1;
    p1.url = "https://a.com";
    p1.html = "AAA";

    Page p2;
    p2.url = "https://b.com";
    p2.html = "BBB";

    Page p3;
    p3.url = "https://c.com";
    p3.html = "CCC";

    db.insert(p1);
    db.insert(p2);
    db.insert(p3);

    EXPECT_EQ(db.get(1).url, "https://a.com");
    EXPECT_EQ(db.get(2).url, "https://b.com");
    EXPECT_EQ(db.get(3).url, "https://c.com");
}