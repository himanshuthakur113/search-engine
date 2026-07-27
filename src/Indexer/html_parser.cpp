#include "Indexer/html_parser.h"

enum class ParserState
{
    TEXT,
    TAG,
    SCRIPT,
    STYLE,
    COMMENT
};

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
            else
            {
                text += html[i];
            }

            break;

        case ParserState::TAG:

            if (html[i] == '>')
            {
                state = ParserState::TEXT;
                text += ' ';
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