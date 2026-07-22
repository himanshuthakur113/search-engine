#include "visited_set.h"

void VisitedSet::markVisited(const std::string& url)
{
    visited.insert(url, 1);
}

bool VisitedSet::contains(const std::string& url)
{
    return visited.contains(url);
}

int VisitedSet::size() const
{
    return visited.size();
}
