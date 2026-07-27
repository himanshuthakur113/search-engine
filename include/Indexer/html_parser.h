#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <string>

class HTMLParser
{
    private:
        std::string decode_entity(const std::string& entity);
    public:
        std::string extract_text(const std::string& html);
};

#endif