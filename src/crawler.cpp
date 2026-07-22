#include "crawler.h"
#include "fetcher.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "url_handler.h"

WebCrawler::WebCrawler()
{
    maxDepth = 0;
    maxPages = 0;
}

WebCrawler::WebCrawler(int _maxDepth,int _maxPages)
{
    maxDepth = _maxDepth;
    maxPages = _maxPages;
}

DA<std::string> WebCrawler::link_extractor(const std::string &html)
{

    DA<std::string> urls;

    for(int i = 0;i+6 < html.size();++i)
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

void WebCrawler::crawl(const CrawlTask& seedTask)
{   
    int pages_crawled = 0;
    Fetcher fetcher;
    Frontier.push(seedTask);
    visited.insert(seedTask.url,1);

    while(!Frontier.empty())
    {   
        if(pagestore.pages_saved() >= maxPages)
        {
            break;
        }

        CrawlTask task = Frontier.pop();

        try{
            std::string html = fetcher.fetch(task.url);
          
            pagestore.save(html);
            std::cout <<"write page" <<std::endl;


            if(task.depth >= maxDepth)
            {
                continue;
            }

            URL handle_url(task.url);
            DA<std::string> links = link_extractor(html);
       
            for(int i = 0;i < links.size();++i)
            {
                std::string resolved_url = handle_url.resolve_url(links[i]);
                resolved_url = handle_url.normalize_url(resolved_url);

                if(visited.contains(resolved_url)){
                    continue;
                }

                Frontier.push(CrawlTask(resolved_url, task.depth + 1));
                visited.insert(resolved_url,1);
            }

        
        }
        catch(const std::exception &e)
        {
        std::cout << "Failed to fetch" <<task.url <<e.what() <<"\n";
        continue;
        }
    }
}