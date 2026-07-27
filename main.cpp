#include "fetcher.h"
#include "crawler.h"
#include "crawl_task.h"
#include <iostream>
#include "page.h"
#include "sqlite_db.h"
#include "include/Indexer/indexer.h"
#include "Indexer/inverted_index.h"
using namespace std;
int main()
{   

   /* Page p;
    p.id = 1;
    p.url = "www.sag.com";
    p.html = "<html?fa";

    SQLiteDB db("../pages.db");
    db.insert(p);*/
    
    /*string url = "https://gcdharamshala.in/";
    CrawlTask seedTask(url,0);
    
    WebCrawler crawler(3,10);

    crawler.crawl(seedTask);    */
    PageStore s;

    Indexer i;
    i.build(s);
    cout << "index build" <<"\n";
    
    InvertedIndex x = i.get_index();

    x.show(10);
}