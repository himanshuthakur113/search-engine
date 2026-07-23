#ifndef PAGE_STORE_H
#define PAGE_STORE_H
#include <string>
#include "page.h"
#include "dynamic_array.h"
#include "sqlite_db.h"

class PageStore
{
    private:
        SQLiteDB db;
        int next_page_id;
        DA<Page> buffer;

    public:
        PageStore(const std::string &dir = "../data");
        void save(Page page);
        int pages_saved() const;
        void flush();

};

#endif