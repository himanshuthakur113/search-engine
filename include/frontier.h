#ifndef FRONTIER_H
#define FRONTIER_H

#include "crawl_task.h"
#include "queue.h"

class Frontier
{
    private:
        queue<CrawlTask> frontier;

    public:
        void push(const CrawlTask &task);
        CrawlTask pop();
        int size() const;
        bool empty() const;

};

#endif