#include "url_handler.h"

URL::URL(const std::string& url)
{
    int i = 0;

    //for scheme
    while(i < url.size() - 2)
    {
        if(url[i] == ':' && url[i+1] == '/' && url[i+2] == '/')
        {
            break;
        }
        scheme += url[i];
        i++;
    }

    i+= 3;

    //for host
    while(i < url.size())
    {
        if(url[i] == ':' || url[i] == '/' || url[i] == '#' || url[i] == '?')
        {
            break;
        }
        host += url[i];
        i++;
    }
    
    //for port
    if(i < url.size() && url[i] == ':')
    {
        i++;
        while(i < url.size())
        {
            if(url[i] == '?' || url[i] == '#' || url[i] == '/')
            {
                break;
            }
            port += url[i];
            i++;
        }
    }

    //for path
    if(i < url.size() && url[i] == '/')
    {
        //here i haven't been incremented because path need to capture the / 
        while(i < url.size())
        {
            if(url[i] == '?' ||url[i] == '#')
            {
                break;
            }
            path += url[i];
            i++;
        }
    }

    //for query
    if(i < url.size() && url[i] == '?')
    {
        i++;
        while(i < url.size())
        {
            if(url[i] == '#'){
                break;
            }
            query += url[i];
            i++;
        }
    }

    //fragment
    if(i < url.size() &&url[i] == '#')
    {
        i++;
        while(i < url.size())
        {
            fragment += url[i];
            i++;
        }
    }


}