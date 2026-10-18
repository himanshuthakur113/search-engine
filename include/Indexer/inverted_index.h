#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include "hash_map.h"
#include <string>
#include "posting_list.h"
#include "dynamic_array.h"

class InvertedIndex
{
    private:
        HashMap<std::string, PostingList> index;

    public:
        void show(int n) const;
        void add_word(const std::string& word,int page_Id,int count = 1);
        bool contains(const std::string& word) const;
        const PostingList* search(const std::string& word) const;
        DA<std::string> giveKey() const;
        
};

#endif