#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include <string>
#include "dynamic_array.h"
#include "page_store.h"
#include "Indexer/inverted_index.h"
#include "Indexer/tokenizer.h"
#include "Indexer/normalizer.h"

struct SearchResult
{
    int page_id;
    std::string url;
};

// Answers queries using an InvertedIndex.
// A page matches only if it contains ALL the words of the query.
// The index and the page store must outlive the SearchEngine.
class SearchEngine
{
    private:
        const InvertedIndex& index;
        PageStore& store;
        Tokenizer tokenizer;
        Normalizer normalizer;

    public:
        SearchEngine(const InvertedIndex& index, PageStore& store);

        // Query text -> normalized, de-duplicated search terms
        // (same pipeline the indexer used, so words match the index)
        DA<std::string> parse_query(const std::string& query);

        // IDs of the pages that contain every term of the query
        DA<int> match(const std::string& query);

        // Same as match(), but with the URL of each page
        DA<SearchResult> search(const std::string& query);
};

#endif
