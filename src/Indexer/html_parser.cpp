#include "html_parser.h"

std::string HTMLParser::extract_text(const std::string& html)
{
    std::string text;

    bool insidetag = false;

    for(char ch:html)
    {
        if(ch == '<')
        {
            insidetag = true;
        }
        
        if(ch == '>')
        {
            insidetag = false;
            text += ' ';
            continue;
        }

        if(!insidetag)
        {
            text += ch;
        }

        return text;
    }
}