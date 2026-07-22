#include "frontier.h"

void Frontier::push(const CrawlTask& task)
{
    frontier.push(task);
}

CrawlTask Frontier::pop()
{
    return frontier.pop();
}

int Frontier::size() const
{
    return frontier.size();
}

bool Frontier::empty() const
{
    return frontier.empty();
}