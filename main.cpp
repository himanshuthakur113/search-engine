#include <iostream>
#include <cstdio>

#include "crawler.h"
#include "crawl_task.h"
#include "page_store.h"
#include "Indexer/indexer.h"

int main()
{


    std::cout << "==============================\n";
    std::cout << "   Web Crawler Test\n";
    std::cout << "==============================\n";

    std::string seed = "https://gcdharamshala.in/";
    CrawlTask task(seed, 0);

    WebCrawler crawler(2, 50);   // depth = 2, max pages = 50

    std::cout << "\n[1] Crawling...\n";
    crawler.crawl(task);

    std::cout << "\nCrawling completed.\n";

    std::cout << "\n==============================\n";
    std::cout << "   Indexer Test\n";
    std::cout << "==============================\n";

    PageStore store;
    Indexer indexer;

    std::cout << "\n[2] Building Inverted Index...\n";
    indexer.build(store);

    std::cout << "Index built successfully.\n";

    std::cout << "\n[3] Sample Keywords\n";
    std::cout << "------------------------------\n";

    indexer.get_index().show(20);

    std::cout << "\n==============================\n";
    std::cout << "Pipeline completed successfully.\n";
    std::cout << "==============================\n";

    return 0;
}