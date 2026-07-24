#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <string>

class HTMLParser
{
    public:
        std::string extract_text(const std::string& html);
};

#endif