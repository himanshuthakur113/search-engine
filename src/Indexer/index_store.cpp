#include "Indexer/index_store.h"
#include "dynamic_array.h"

IndexStore::IndexStore(const std::string& filename):db(filename + "/index.db")
{  
    db.initialize_index_table();
}

void IndexStore::save(const InvertedIndex& index)
{
    db.clear_index_table();

    DA<std::string> keywords = index.giveKey();
    std::cout << "Keywords = " << keywords.size() << '\n';
    for(int i = 0;i < keywords.size();++i)
    {
        const PostingList* postingList = index.search(keywords[i]);
        const DA<Posting>& postings = postingList->getPostings();
        db.beginTransaction();
        try
        {
            for(int j = 0;j < postings.size();++j)
            {   
                db.insert_index(keywords[i],postings[j].page_id);
            }
        db.commitTransaction();
        }
        catch(...)
        {
            db.rollbackTransaction();
            throw;
        }
    }
}

InvertedIndex IndexStore::load()
{
    InvertedIndex index;
    DA<IndexRow> rows = db.get_all_index_rows();
    for(int i = 0;i < rows.size();++i)
    {
        index.add_word(rows[i].keyword, rows[i].page_id);
    }
    return index;
}
