#include "indexer.h"
#include "dynamic_array.h"

void Indexer::index_page(const Page& page)
{
    std::string text = parser.extract_text(page.html);
    DA<std::string> tokens = tokenizer.tokenize(text);

    for(int i = 0;i < tokens.size(); ++i)
    {
        std::string word = normalizer.normalize(tokens[i]);
        if(!word.empty())
        {
            index.add_word(word, page.id);
        }
    }
}