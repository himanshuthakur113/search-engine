#ifndef INDEXER_H
#define INDEXER_H

#include "Indexer/html_parser.h"
#include "Indexer/tokenizer.h"
#include "Indexer/normalizer.h"
#include "Indexer/inverted_index.h"
#include "page.h"
#include "page_store.h"
#include "Indexer/index_store.h"

class Indexer
{
private:
    HTMLParser parser;
    Tokenizer tokenizer;
    Normalizer normalizer;
    InvertedIndex index;
    IndexStore i_store;
    void index_page(const Page& page);

public:

    const InvertedIndex& get_index() const;
    void build(PageStore& store);
};

#endif