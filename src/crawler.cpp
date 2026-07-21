#include "crawler.h"
#include "fetcher.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "url_handler.h"

WebCrawler::WebCrawler()
{
    maxDepth = 0;
    maxPages = 10;
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
    int mp = 0;
    Fetcher fetcher;
    Frontier.push(seedURL);
    visited.insert(seedURL,1);

    std::filesystem::create_directory("pages");

    while(!Frontier.empty())
    {   
        if(mp == maxPages)
        {
            break;
        }
        std::string url = Frontier.pop();
        URL handle_url(url);

        try{
        std::string html = fetcher.fetch(url);
        

        std::ofstream file("../pages/page_" + std::to_string(mp) + ".txt");
        if(!file.is_open())
        {
            std::cout << "Failed to open file\n";
        }

        file << html;
        std::cout <<"write page" <<std::endl;

        DA<std::string> urls = link_extractor(html);
       
        for(int i = 0;i < urls.size();++i)
        {
            std::string url_ = handle_url.resolve_url(urls[i]);
        
            if(visited.contains(url_)){
                continue;
            }
            Frontier.push(url_);
            visited.insert(url_,1);
        }

        mp++;
    }
    catch(const std::exception &e)
    {
        std::cout << "Failed to fetch" <<url <<e.what() <<"\n";
        continue;
    }
    }
}

std::string WebCrawler::normalize_url(std::string &url,std::string &baseURL)
{
  return "i am do da do da do da o la la la do da";

}