#include <string>
#include "fetcher.h"
#include "crawl_task.h"
#include "hash_map.h"
#include "queue.h"
#include "dynamic_array.h"
#include "url_handler.h"
#include "page_store.h"
#include "frontier.h"
#include "visited_set.h"

class WebCrawler
{   
    private:
        int processed_pages;
        int maxDepth;
        int maxPages;
        Fetcher fetcher;
        Frontier frontier;
        VisitedSet visited;
        PageStore pagestore;
       
    public:
        WebCrawler();
        WebCrawler(int _maxDepth,int _maxPages);
        void crawl(const CrawlTask& seedTask);
        DA<std::string> link_extractor(const std::string &html);
};