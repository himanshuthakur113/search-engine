#include "page_store.h"
#include "sqlite3.h"
#include <stdexcept>

PageStore::PageStore(const std::string &dir):
    db(dir + "/pages.db"),
    next_page_id(0)
   {}


void PageStore::save(Page page)
{   
    if(db.exists(page.url))
    {
        return;
    }
    page.id = next_page_id;
    next_page_id++;

    buffer.append(page);
    
}

bool PageStore::isCached(const std::string &url)
{
    return (db.exists(url));
}

std::string PageStore::fetch(const std::string &url)
{
    return db.fetch(url);
}

void PageStore::flush()
{
    for(int i = 0;i < buffer.size(); ++i)
    {
        db.insert(buffer[i]);
    }
    buffer.clear();
}

int PageStore::pages_saved() const
{
    return buffer.size();
}