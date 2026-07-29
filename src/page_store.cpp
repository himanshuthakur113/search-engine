#include "page_store.h"
#include "sqlite3.h"
#include <stdexcept>

PageStore::PageStore(const std::string& dir)
    : db(dir + "/pages.db")
{
    db.initialize_page_table();
}


void PageStore::save(Page page)
{   
    if(db.exists(page.url) || buffered_urls.contains(page.url))
    {
        return;
    }
    page.id = next_page_id;
    next_page_id++;

    buffer.append(page);
    buffered_urls.insert(page.url, 1);

    if(buffer.size() >= 10)
    {
        flush();
    }
}

bool PageStore::isCached(const std::string &url)
{
    return (db.exists(url) || buffered_urls.contains(url));
}

std::string PageStore::fetch(const std::string &url)
{
    return db.fetch(url);
}

void PageStore::flush()
{
    if(buffer.size() == 0)
    {
        return;
    }

    try
    {
        db.beginTransaction();
        for(int i = 0;i < buffer.size(); ++i)
        {
            db.insert(buffer[i]);
        }
        db.commitTransaction();
    }
    catch(...)
    {
        db.rollbackTransaction();
        throw;
    }
    saved_pages += buffer.size();
    buffer.clear();
    buffered_urls.clear();
}

int PageStore::pages_saved() const
{
    return saved_pages;
}

int PageStore::buffered_pages() const
{
    return buffer.size();
}

DA<Page> PageStore::get_all_pages()
{   
    flush();
    return db.get_all_pages();
}
