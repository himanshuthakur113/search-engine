#ifndef INDEXER_H
#define INDEXER_H

#include "html_parser.h"
#include "tokenizer.h"
#include "normalizer.h"
#include "inverted_index.h"
#include "page.h"
#include "page_store.h"

class Indexer
{
private:
    HTMLParser parser;
    Tokenizer tokenizer;
    Normalizer normalizer;
    InvertedIndex index;

public:
    void index_page(const Page& page);
    const InvertedIndex& get_index() const;
    void build(PageStore& store);
};

#endif