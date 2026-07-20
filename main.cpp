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
    DA<string> urls = crawler.link_extractor(res);
    for(int i = 0;i < urls.size(); ++i)
    {
        cout << urls[i] <<endl;
    }

    
}