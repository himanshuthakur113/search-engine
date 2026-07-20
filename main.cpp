#include "fetcher.h"
#include "crawler.h"
#include <iostream>
using namespace std;
int main()
{
    Fetcher fetcher;
    string url = "https://gcdharamshala.in/";
    string res = fetcher.fetch(url);
    
    WebCrawler crawler;
    crawler.crawl(url);
}