cmake_minimum_required(VERSION 3.15)

project(WebCrawler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# ----------------------------------------------------
# Packages
# ----------------------------------------------------

find_package(CURL REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(GTest REQUIRED)

# ----------------------------------------------------
# Data Structures Library
# ----------------------------------------------------

add_subdirectory(
    ../../Project_1/data_structures-library
    data_structures_build
)

# ----------------------------------------------------
# Main Executable
# ----------------------------------------------------

add_executable(main
    main.cpp

    src/fetcher.cpp
    src/crawler.cpp
    src/url_handler.cpp
    src/frontier.cpp
    src/page_store.cpp
    src/visited_set.cpp
    src/crawl_task.cpp
    src/sqlite_db.cpp

    src/Indexer/indexer.cpp
    src/Indexer/html_parser.cpp
    src/Indexer/tokenizer.cpp
    src/Indexer/normalizer.cpp
    src/Indexer/inverted_index.cpp
    src/Indexer/posting_list.cpp
    src/Indexer/index_store.cpp
)

target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(main PRIVATE
    CURL::libcurl
    SQLite::SQLite3
    DataStructuresLibrary
)

# ----------------------------------------------------
# Test Executable
# ----------------------------------------------------

add_executable(test_runner
    #test/test_url_resolver.cpp
    #test/test_url_normalizer.cpp
    #test/test_sqlite_db.cpp
    #test/test_pageStore.cpp
    #test/test_fetcher.cpp
    #test/test_extractlinks.cpp
    #test/test_frontier.cpp
    #test/test_visitedSet.cpp
    #test/test_crawler.cpp

    # Indexer Tests
    test/test_html_parser.cpp
    #test/test_tokenizer.cpp
    #test/test_normalizer.cpp
    #test/test_posting_list.cpp
    #test/test_inverted_index.cpp
    #test/test_index_store.cpp
    #test/test_indexer.cpp

    src/fetcher.cpp
    src/crawler.cpp
    src/url_handler.cpp
    src/frontier.cpp
    src/page_store.cpp
    src/visited_set.cpp
    src/crawl_task.cpp
    src/sqlite_db.cpp

    # Indexer Sources
    src/Indexer/indexer.cpp
    src/Indexer/html_parser.cpp
    src/Indexer/tokenizer.cpp
    src/Indexer/normalizer.cpp
    src/Indexer/inverted_index.cpp
    src/Indexer/posting_list.cpp
    src/Indexer/index_store.cpp
)

target_include_directories(test_runner PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(test_runner PRIVATE
    GTest::gtest
    GTest::gtest_main

    CURL::libcurl
    SQLite::SQLite3
    DataStructuresLibrary
)

# ----------------------------------------------------
# Testing
# ----------------------------------------------------

enable_testing()

include(GoogleTest)

gtest_discover_tests(test_runner)