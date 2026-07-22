#include "fetcher.h"
#include "crawler.h"
#include "crawl_task.h"
#include <iostream>
using namespace std;
int main()
{
 
    string url = "https://gcdharamshala.in/";
    CrawlTask seedTask(url,0);
    
    WebCrawler crawler(3,100);

    crawler.crawl(seedTask);
}