#include "page_store.h"

#include <fstream>
#include <filesystem>
#include <stdexcept>

PageStore::PageStore(const std::string &dir)
{
    directory = dir;
    next_page_id = 0;
    try {
        std::filesystem::create_directories(directory);
    } catch (...) {}
}

void PageStore::save(Page page)
{
    page.id = next_page_id;
    next_page_id++;

    buffer.append(page);
    
}

void PageStore::flush()
{
    std::ofstream metadata(directory + "/metadata.txt",std::ios::app);
    for(int i = 0;i < buffer.size(); ++i)
    {
        std::string filepath = directory + "/page_" +  std::to_string(buffer[i].id) + ".txt";
        std::ofstream file(filepath);
        if(!file.is_open())
        {
            throw std::runtime_error("failed to create file: " + filepath);
        }

        file << buffer[i].html;
        metadata << buffer[i].id << " " << buffer[i].url << "\n";
    }
    buffer.clear();
}

int PageStore::pages_saved() const
{
    return buffer.size();
}