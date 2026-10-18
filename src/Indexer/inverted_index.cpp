#include "Indexer/inverted_index.h"
#include "dynamic_array.h"

void InvertedIndex::add_word(const std::string& word,int page_Id,int count)
{
    index.get(word).add_posting(page_Id,count);
}

bool InvertedIndex::contains(const std::string& word) const
{
    return index.contains(word);
}

const PostingList* InvertedIndex::search(const std::string& word) const
{
    return index.find(word);
}

void InvertedIndex::show(int n) const
{
    index.show(n);
}

DA<std::string> InvertedIndex::giveKey() const
{
    DA<std::string> keywords = index.keys();
    return keywords;
}
