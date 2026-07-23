#ifndef SQL_H
#define SQL_H

#include <sqlite3.h>
#include <string>
#include "page.h"

class SQLiteDB
{
    private:
        sqlite3* db;

    public:
        SQLiteDB(const std::string& filename);
        ~SQLiteDB();
        void insert(const Page& page);
        Page get(int id);
        bool exists(const std::string &url);
};

#endif