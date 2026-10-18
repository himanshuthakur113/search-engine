#include "Search/search_engine.h"

SearchEngine::SearchEngine(const InvertedIndex& index, PageStore& store)
    : index(index), store(store)
{
}

DA<std::string> SearchEngine::parse_query(const std::string& query)
{
    DA<std::string> raw_tokens = tokenizer.tokenize(query);
    DA<std::string> terms;

    for(int i = 0;i < raw_tokens.size(); ++i)
    {
        std::string term = normalizer.normalize(raw_tokens[i]);
        if(term.empty())
        {
            continue;
        }

        bool already_present = false;
        for(int j = 0;j < terms.size(); ++j)
        {
            if(terms[j] == term)
            {
                already_present = true;
                break;
            }
        }
        if(!already_present)
        {
            terms.append(term);
        }
    }
    return terms;
}

DA<int> SearchEngine::match(const std::string& query)
{
    DA<int> result;
    DA<std::string> terms = parse_query(query);

    if(terms.size() == 0)
    {
        return result;
    }

    // A word that is not in the index means no page can match all words.
    // While checking, remember the shortest posting list: scanning it
    // keeps the work small.
    const PostingList* shortest = nullptr;
    for(int i = 0;i < terms.size(); ++i)
    {
        const PostingList* postings = index.search(terms[i]);
        if(postings == nullptr)
        {
            return result;
        }
        if(shortest == nullptr ||
           postings->getPostings().size() < shortest->getPostings().size())
        {
            shortest = postings;
        }
    }

    const DA<Posting>& candidates = shortest->getPostings();
    for(int i = 0;i < candidates.size(); ++i)
    {
        int page_id = candidates[i].page_id;

        bool in_all = true;
        for(int j = 0;j < terms.size(); ++j)
        {
            if(!index.search(terms[j])->contains(page_id))
            {
                in_all = false;
                break;
            }
        }
        if(in_all)
        {
            result.append(page_id);
        }
    }
    return result;
}

DA<SearchResult> SearchEngine::search(const std::string& query)
{
    DA<int> page_ids = match(query);
    DA<SearchResult> results;

    for(int i = 0;i < page_ids.size(); ++i)
    {
        SearchResult r;
        r.page_id = page_ids[i];
        r.url = store.url_of(page_ids[i]);
        results.append(r);
    }
    return results;
}
