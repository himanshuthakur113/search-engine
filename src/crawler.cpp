#include "crawler.h"

WebCrawler::WebCrawler()
{
    maxDepth = 0;
    maxPages = 0;
}

DA<std::string> WebCrawler::link_extractor(const std::string &html)
{

    DA<std::string> urls;

    for(int i = 0;i < html.size()-6;++i)
    {
        if(html[i] == '<' && html[i+1] == 'a' && html[i+2] == ' ')
        {
            if(html[i+3] == 'h' && html[i+4] == 'r' && html[i+5] == 'e' && html[i+6] == 'f')
            {   
                i += 7;
                std::string url;
                while(html[i] != '>')
                {
                    url += html[i];
                    i++;
                }
                urls.append(url);
            }
        }
    }
    return urls;
}

void WebCrawler::crawl(const std::string& seedURL)
{

}
