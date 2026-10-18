#ifndef SQL_H
#define SQL_H

#include <sqlite3.h>
#include <string>
#include "page.h"
#include "dynamic_array.h"

struct IndexRow
{
    std::string keyword;
    int page_id;
    int count;
};

class SQLiteDB
{
    private:
        sqlite3* db;

    public:
        SQLiteDB(const std::string& filename);
        ~SQLiteDB();

        void initialize_page_table();
        void initialize_index_table();

        void insert(const Page& page);
        std::string fetch(const std::string& url);
        Page get(int id);
        std::string get_url(int id);
        bool exists(const std::string &url);
        DA<Page> get_all_pages();

        void beginTransaction();
        void commitTransaction();
        void rollbackTransaction();

        void insert_index(const std::string& keyword,int page_id,int count);
        void clear_index_table();
        DA<IndexRow> get_all_index_rows();
};
#endif