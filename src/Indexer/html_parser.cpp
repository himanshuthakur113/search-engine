#include "Indexer/html_parser.h"

enum class ParserState
{
    TEXT,
    TAG,
    SCRIPT,
    STYLE,
    COMMENT
};

std::string HTMLParser::decode_entity(const std::string& entity)
{
    if(entity == "&amp;")
    {
        return "&";
    }
    if(entity == "&lt;")
    {
        return "<";
    }
    if(entity == "&gt;")
    {
        return ">";
    }
    if(entity == "&quot;")
    {
        return "\"";
    }
    if(entity == "&apos;")
    {
        return "'";
    }
    if(entity == "&nbsp;")
    {
        return " ";
    }
    return "";
}


std::string HTMLParser::extract_text(const std::string& html)
{
    std::string text;
    ParserState state = ParserState::TEXT;

    for (int i = 0; i < html.size(); ++i)
    {
        switch (state)
        {
        case ParserState::TEXT:

            if (html.compare(i, 4, "<!--") == 0)
            {
                state = ParserState::COMMENT;
                i += 3;
            }
            else if (html.compare(i, 7, "<script") == 0)
            {
                state = ParserState::SCRIPT;
                i += 6;
            }
            else if (html.compare(i, 6, "<style") == 0)
            {
                state = ParserState::STYLE;
                i += 5;
            }
            else if (html[i] == '<')
            {
                state = ParserState::TAG;
            }
            else if(html[i] == '&')
            {
                std::string entity;
                int j = i;

                while(j < html.size() && html[j] != ';')
                {
                    entity += html[j];
                    j++;
                }
                if(j < html.size())
                {
                    entity += ';';
                    std::string decoded = decode_entity(entity);
                    if(!decoded.empty())
                    {
                        text += decoded;
                        i = j;
                    }
                    else
                    {
                        text += '&';
                    }
                }
                else
                {
                    text += '&';
                }
            } 
            else
            {   
                if(std::isspace(html[i]))
                {
                    if(!text.empty() && text.back() != ' ')
                    {
                        text += ' ';
                    }
                }
                else
                {
                text += html[i];
                }
            }

            break;

        case ParserState::TAG:

            if (html[i] == '>')
            {
                state = ParserState::TEXT;
                
                if(!text.empty() && text.back() != ' ')
                {
                    text += ' ';
                }
            }

            break;

        case ParserState::SCRIPT:

            if (html.compare(i, 9, "</script>") == 0)
            {
                state = ParserState::TEXT;
                i += 8;
            }

            break;

        case ParserState::STYLE:

            if (html.compare(i, 8, "</style>") == 0)
            {
                state = ParserState::TEXT;
                i += 7;
            }

            break;

        case ParserState::COMMENT:

            if (html.compare(i, 3, "-->") == 0)
            {
                state = ParserState::TEXT;
                i += 2;
            }

            break;
        }
    }

    return text;
}