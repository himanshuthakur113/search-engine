#include "Indexer/html_parser.h"

enum class ParserState
{
    TEXT,
    TAG,
    SCRIPT,
    STYLE,
    COMMENT
};

bool HTMLParser::match_tag(const std::string &html,int index,const std::string& tag)
{   
    if(index + tag.size() > html.size())
    {
        return false;
    }

    for(int i = 0;i < tag.length();++i)
    {
        if(std::tolower(html[index + i]) != tag[i])
        {
            return false;
        }
    }
    
    //at the end of string they match
    if(index + tag.size() == html.size())
    {
        return true;
    }
    
    char next = html[index + tag.size()];
    return next == '>'|| std::isspace(next);
}

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
            else if (match_tag(html, i, "<script"))
            {
                state = ParserState::SCRIPT;
                i += 6;
            }
            else if (match_tag(html, i, "<style"))
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

            if (match_tag(html,i,"</script>"))
            {
                state = ParserState::TEXT;
                i += 8;
            }

            break;

        case ParserState::STYLE:

            if (match_tag(html, i, "</style>"))
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
    while(!text.empty() && text.back() == ' ')
    {
    text.pop_back();
    }
    return text;
}