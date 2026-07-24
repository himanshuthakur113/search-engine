#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "dynamic_array.h"
#include <string>

class Tokenizer
{
    public:
        DA<std::string> tokenize(const std::string &text);
};

#endif