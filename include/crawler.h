#include <string>
#include "hash_map.h"
#include "queue.h"

struct CrawlTask
{
    std::string url;
    int depth;
};

class WebCrawler
{   
    private:
        int maxDepth;
        int maxPages;
        queue<CrawlTask> Frontier;
        HashMap<std::string,int> visited;


    public:
        WebCrawler();
        void crawl(const std::string& seedURL);
};