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

include(FetchContent)

FetchContent_Declare(
    dslib
    GIT_REPOSITORY https://github.com/himanshuthakur113/data_structures-library.git
    GIT_TAG        v0.1.0
)

# To use a local copy of the library instead of downloading it, configure with:
#   cmake -S . -B build -DFETCHCONTENT_SOURCE_DIR_DSLIB=/path/to/data_structures-library
FetchContent_MakeAvailable(dslib)

# ----------------------------------------------------
# Core library
# (everything except the executables' main() files and the tests,
#  so main, search and test_runner share one list of sources)
# ----------------------------------------------------

add_library(engine_core STATIC
    src/fetcher.cpp
    src/crawler.cpp
    src/url_handler.cpp
    src/frontier.cpp
    src/page_store.cpp
    src/visited_set.cpp
    src/crawl_task.cpp
    src/sqlite_db.cpp

    # Indexer
    src/Indexer/indexer.cpp
    src/Indexer/html_parser.cpp
    src/Indexer/tokenizer.cpp
    src/Indexer/normalizer.cpp
    src/Indexer/inverted_index.cpp
    src/Indexer/posting_list.cpp
    src/Indexer/index_store.cpp
)

target_include_directories(engine_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(engine_core PUBLIC
    CURL::libcurl
    SQLite::SQLite3
    DataStructuresLibrary
)

# ----------------------------------------------------
# Executables
# ----------------------------------------------------

# crawl pages and build the index
add_executable(main main.cpp)
target_link_libraries(main PRIVATE engine_core)

# ----------------------------------------------------
# Test Executable
# ----------------------------------------------------

add_executable(test_runner
    test/test_url_resolver.cpp
    test/test_url_normalizer.cpp
    test/test_sqlite_db.cpp
    test/test_pageStore.cpp
    test/test_fetcher.cpp
    test/test_extractlinks.cpp
    test/test_frontier.cpp
    test/test_visitedSet.cpp
    test/test_crawler.cpp

    # Indexer Tests
    test/test_html_parser.cpp
    test/test_tokenizer.cpp
    test/test_normalizer.cpp
    test/test_posting_list.cpp
    test/test_inverted_index.cpp
    test/test_index_store.cpp
)

target_link_libraries(test_runner PRIVATE
    engine_core
    GTest::gtest
    GTest::gtest_main
)

# ----------------------------------------------------
# Testing
# ----------------------------------------------------

enable_testing()

include(GoogleTest)

gtest_discover_tests(test_runner)
