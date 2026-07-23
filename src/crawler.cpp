#include "crawler.h"
#include "fetcher.h"
#include <iostream>
#include <cctype>
#include "url_handler.h"
#include "page.h"

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
    int len = html.size();
    int i = 0;

    while (i < len)
    {
        if (html[i] == '<' && (i+1 < len) && (html[i+1] == 'a' || html[i+1] == 'A'))
        {
            if (i+2 < len && (html[i+2] == ' ' || html[i+2] == '>'))
            {
                i += 2;
                while (i < len && html[i] != '>')
                {
                    while (i < len && html[i] == ' ')
                    {
                        i++;
                    }
                    if (i >= len || html[i] == '>') break;

                    if (i + 4 <= len &&
                        std::tolower(html[i]) == 'h' &&
                        std::tolower(html[i+1]) == 'r' &&
                        std::tolower(html[i+2]) == 'e' &&
                        std::tolower(html[i+3]) == 'f')
                    {
                        i += 4;
                        while (i < len && html[i] == ' ') i++;
                        if (i < len && html[i] == '=')
                        {
                            i++;
                            while (i < len && html[i] == ' ') i++;

                            std::string url;
                            if (i < len && (html[i] == '"' || html[i] == '\''))
                            {
                                char quote = html[i];
                                i++;
                                int start = i;
                                while (i < len && html[i] != quote && html[i] != '>')
                                {
                                    i++;
                                }
                                url = html.substr(start, i - start);
                                if (i < len && html[i] == quote) i++;
                            }
                            else
                            {
                                int start = i;
                                while (i < len && html[i] != ' ' && html[i] != '>')
                                {
                                    i++;
                                }
                                url = html.substr(start, i - start);
                            }

                            if (!url.empty())
                            {
                                urls.append(url);
                            }
                        }
                    }
                    else
                    {
                        while (i < len && html[i] != ' ' && html[i] != '>')
                        {
                            i++;
                        }
                    }
                }
            }
        }
        i++;
    }
    return urls;
}

void WebCrawler::crawl(const CrawlTask& seedTask)
{   

    frontier.push(seedTask);
    visited.markVisited(seedTask.url);

    while(!frontier.empty())
    {   
        if(pagestore.pages_saved() >= maxPages)
        {
            break;
        }

        CrawlTask task = frontier.pop();

        try{

            std::string html;
            
            if(pagestore.isCached(task.url))
            {
                html = pagestore.fetch(task.url);
                std::cout << "Write through DB" <<"\n";
            }
            else
            {
                html = fetcher.fetch(task.url);
                Page page;
                page.url = task.url;
                page.html = html;

                pagestore.save(page); //if db has page it didn't store again
                std::cout <<"write page" <<std::endl;

            }  

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

                frontier.push(CrawlTask(resolved_url, task.depth + 1));
                visited.markVisited(resolved_url);
            }  
        }
        catch(const std::exception &e)
        {
        std::cout << "Failed to fetch" <<task.url <<e.what() <<"\n";
        continue;
        }
    }
    pagestore.flush();
}