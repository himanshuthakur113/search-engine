#include <iostream>
#include <iomanip>
#include <string>

#include "page_store.h"
#include "Indexer/index_store.h"
#include "Search/search_engine.h"

// How many of the best results are shown for each query
const int MAX_RESULTS_SHOWN = 10;

// Usage: ./search [data_dir]      (default data_dir: ../data)
// Run ./main first, so that pages.db and index.db exist.
int main(int argc, char* argv[])
{
    std::string dir = (argc > 1) ? argv[1] : "../data";

    try
    {
        PageStore store(dir);
        IndexStore index_store(dir);
        InvertedIndex index = index_store.load();

        int keywords = index.giveKey().size();
        if(keywords == 0)
        {
            std::cerr << "The index in '" << dir << "' is empty (or was made by an older version).\n"
                      << "Run ./main to crawl pages and build the index.\n";
            return 1;
        }

        SearchEngine engine(index, store);

        std::cout << "\nLoaded " << keywords << " keywords. "
                  << "Type a query (or 'exit' to quit).\n";

        std::string query;
        while(true)
        {
            std::cout << "\nsearch> ";
            if(!std::getline(std::cin, query))
            {
                break;
            }
            if(query == "exit" || query == "quit")
            {
                break;
            }

            DA<std::string> terms = engine.parse_query(query);
            if(terms.size() == 0)
            {
                std::cout << "Nothing to search for (empty query, or only very common words).\n";
                continue;
            }

            DA<SearchResult> results = engine.search(query);
            if(results.size() == 0)
            {
                std::cout << "No pages contain all of those words.\n";
                continue;
            }

            int shown = results.size();
            if(shown > MAX_RESULTS_SHOWN)
            {
                shown = MAX_RESULTS_SHOWN;
            }

            std::cout << results.size() << " page(s) found";
            if(shown < results.size())
            {
                std::cout << ", showing the best " << shown;
            }
            std::cout << ":\n";

            for(int i = 0;i < shown; ++i)
            {
                std::cout << "  " << std::setw(2) << (i + 1) << ". "
                          << std::fixed << std::setprecision(2) << std::setw(6)
                          << results[i].score << "  " << results[i].url << "\n";
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
