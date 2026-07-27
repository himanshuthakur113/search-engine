#include "Indexer/normalizer.h"
#include <cctype>

std::string Normalizer::normalize(const std::string& word)
{   
    std::string l_w = word;
    for(auto& ch:l_w)
    {
        ch = std::tolower(ch);
    }
    return l_w;
}