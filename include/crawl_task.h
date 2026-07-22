#ifndef CRAWLTASK_H
#define CRAWLTASK_H

#include <string>
struct CrawlTask
{
    std::string url;
    int depth;

    CrawlTask(std::string _url,int _depth);
};

#endif