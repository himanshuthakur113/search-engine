#ifndef VISITEDSET_H
#define VISITEDSET_H

#include <string>
#include "hash_map.h"

class VisitedSet
{
private:
    HashMap<std::string, int> visited;

public:
    void markVisited(const std::string& url);
    bool contains(const std::string& url);
    int size() const;
};

#endif