#include "Indexer/tokenizer.h"

DA<std::string> Tokenizer::tokenize(const std::string &text)
{
    std::string token;
    DA<std::string> tokens;

    for(int i = 0;i < text.size(); ++i)
    {   
        if(std::isalnum(text[i]))
        {
            token += text[i];
        }
        else
        {
            if(!token.empty())
            {
                tokens.append(token);
                token = "";
            }
        }
    }
    if(!token.empty())
    {
        tokens.append(token);
    }
    return tokens;
}