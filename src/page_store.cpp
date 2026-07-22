#include "page_store.h"

#include <fstream>
#include <filesystem>
#include <stdexcept>

PageStore::PageStore(const std::string &dir)
{
    directory = dir;
    page_cnt = 0;
}

void PageStore::save(const std::string &html)
{
    std::ofstream file("../pages/page_" + std::to_string(page_cnt) + ".txt");
    if(!file.is_open())
    {
        throw std::runtime_error("failed to create file");
    }

    file << html;
    page_cnt++;
}

int PageStore::pages_saved() const
{
    return page_cnt;
}