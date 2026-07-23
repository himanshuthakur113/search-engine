#include "page_store.h"


#include <stdexcept>

PageStore::PageStore(const std::string &dir):
    db(dir + "/pages.db"),
    next_page_id(0)
   {}


void PageStore::save(Page page)
{
    page.id = next_page_id;
    next_page_id++;

    buffer.append(page);
    
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