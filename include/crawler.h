#include <string>
#include "hash_map.h"
#include "queue.h"
#include "dynamic_array.h"
#include "url_handler.h"
#include "page_store.h"

struct CrawlTask
{
    std::string url;
    int depth;

    CrawlTask(std::string _url,int _depth) : url(_url), depth(_depth) {}
};

class WebCrawler
{   
    private:
        int maxDepth;
        int maxPages;
        queue<CrawlTask> Frontier;
        HashMap<std::string,int> visited;
        PageStore pagestore;
       
    public:
        WebCrawler();
        WebCrawler(int _maxDepth,int _maxPages);
        void crawl(const CrawlTask& seedTask);
        DA<std::string> link_extractor(const std::string &html);
};