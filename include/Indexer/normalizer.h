#ifndef NORMALIZER_H
#define NORMALIZER_H

#include <string>
#include <hash_map.h>

class Normalizer
{
    public:
        Normalizer();
        std::string normalize(const std::string& word);
    private:
        HashMap<std::string,bool> stopwords;
        void load_stopwords();
        std::string to_lowercase(const std::string& word);
        std::string trim_punctuation(const std::string& word);
        bool is_stopWord(const std::string& word);
        bool has_alpha(const std::string& word);
};      

#endif