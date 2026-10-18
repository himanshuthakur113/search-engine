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
    double score;
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

        // Same pages as match(), with their URLs and a relevance score,
        // best match first (equal scores: lower page id first).
        //
        // Score = sum over the query terms of  count * idf
        //   count = how many times the term appears on the page
        //   idf   = ln(1 + total_pages / pages_containing_term)
        // so a word that is rare across all pages counts for more than a
        // word (like a menu item) that appears on almost every page.
        DA<SearchResult> search(const std::string& query);
};

#endif
