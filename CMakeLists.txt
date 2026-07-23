cmake_minimum_required(VERSION 3.15)

project(WebCrawler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

find_package(CURL REQUIRED)
find_package(SQLite3 REQUIRED)


add_subdirectory(
    ../../Project_1/data_structures-library
    data_structures_build
)

add_executable(main main.cpp src/fetcher.cpp src/crawler.cpp src/url_handler.cpp src/frontier.cpp src/page_store.cpp src/visited_set.cpp src/crawl_task.cpp src/sqlite_db.cpp)

target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(main PRIVATE
    CURL::libcurl
    DataStructuresLibrary
    SQLite::SQLite3
)