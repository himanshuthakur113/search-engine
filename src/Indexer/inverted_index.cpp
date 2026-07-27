#include "inverted_index.h"

void InvertedIndex::add_word(const std::string& word,int page_Id)
{
    index.get(word).add_posting(page_Id);
}

bool InvertedIndex::contains(const std::string& word) const
{
    return index.contains(word);
}

const PostingList* InvertedIndex::search(const std::string& word) const
{
    return index.find(word);
}