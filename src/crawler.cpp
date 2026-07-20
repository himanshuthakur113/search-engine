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
            i += 3;
            if(html[i] == 'h' && html[i+1] == 'r' && html[i+2] == 'e' && html[i+3] == 'f')
            {   
                i += 4;
                while(html[i] != '"' && html[i] != '\'')
                {
                    i++;
                }
                char start = html[i];
                i++;
                std::string url;
                while(html[i] != start)
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
