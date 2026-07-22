#include "page_store.h"

#include <fstream>
#include <filesystem>
#include <stdexcept>

PageStore::PageStore(const std::string &dir)
{
    directory = dir;
    page_cnt = 0;
    try {
        std::filesystem::create_directories(directory);
    } catch (...) {}
}

void PageStore::save(const std::string &html)
{
    std::filesystem::create_directories(directory);
    std::string filepath = directory + "/page_" + std::to_string(page_cnt) + ".txt";
    std::ofstream file(filepath);
    if(!file.is_open())
    {
        throw std::runtime_error("failed to create file: " + filepath);
    }

    file << html;
    page_cnt++;
}

int PageStore::pages_saved() const
{
    return page_cnt;
}