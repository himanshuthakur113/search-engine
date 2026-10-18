#include "Search/search_engine.h"
#include <cmath>

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

// true if `a` should be listed before `b`
static bool ranks_before(const SearchResult& a, const SearchResult& b)
{
    if(a.score != b.score)
    {
        return a.score > b.score;
    }
    return a.page_id < b.page_id;
}

// Merge sort: sorts results[left, right) best match first
static void sort_results(DA<SearchResult>& results, DA<SearchResult>& temp, int left, int right)
{
    if(right - left < 2)
    {
        return;
    }

    int middle = left + (right - left) / 2;
    sort_results(results, temp, left, middle);
    sort_results(results, temp, middle, right);

    int i = left;
    int j = middle;
    int k = left;
    while(i < middle && j < right)
    {
        if(ranks_before(results[j], results[i]))
        {
            temp[k++] = results[j++];
        }
        else
        {
            temp[k++] = results[i++];
        }
    }
    while(i < middle)
    {
        temp[k++] = results[i++];
    }
    while(j < right)
    {
        temp[k++] = results[j++];
    }
    for(int n = left; n < right; ++n)
    {
        results[n] = temp[n];
    }
}

DA<SearchResult> SearchEngine::search(const std::string& query)
{
    DA<int> page_ids = match(query);
    DA<SearchResult> results;

    if(page_ids.size() == 0)
    {
        return results;
    }

    DA<std::string> terms = parse_query(query);
    double total_pages = store.total_pages();

    // idf of each term: rare words are worth more
    DA<double> idf;
    for(int t = 0;t < terms.size(); ++t)
    {
        double pages_with_term = index.search(terms[t])->getPostings().size();
        idf.append(std::log(1.0 + total_pages / pages_with_term));
    }

    for(int i = 0;i < page_ids.size(); ++i)
    {
        SearchResult r;
        r.page_id = page_ids[i];
        r.url = store.url_of(page_ids[i]);
        r.score = 0;
        for(int t = 0;t < terms.size(); ++t)
        {
            r.score += index.search(terms[t])->count_for(page_ids[i]) * idf[t];
        }
        results.append(r);
    }

    DA<SearchResult> temp = results;
    sort_results(results, temp, 0, results.size());
    return results;
}
