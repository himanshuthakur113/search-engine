#ifndef INDEX_STORE_H
#define INDEX_STORE_H
#include <string>
#include "sqlite_db.h"
#include "Indexer/inverted_index.h"

class IndexStore
{
    private:
        SQLiteDB db;
    public:
        IndexStore(const std::string& dir = "../data");
        void save(const InvertedIndex& index) ;
};

#endif