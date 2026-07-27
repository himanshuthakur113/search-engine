#ifndef SQL_H
#define SQL_H

#include <sqlite3.h>
#include <string>
#include "page.h"
#include "dynamic_array.h"

class SQLiteDB
{
    private:
        sqlite3* db;

    public:
        SQLiteDB(const std::string& filename);
        ~SQLiteDB();
        void insert(const Page& page);
        std::string fetch(const std::string& url);
        Page get(int id);
        bool exists(const std::string &url);
        void beginTransaction();
        void commitTransaction();
        void rollbackTransaction();
        DA<Page> get_all_pages();
};

#endif