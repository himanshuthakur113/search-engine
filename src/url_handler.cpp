#include "url_handler.h"

URL::URL(const std::string& baseURL)
{
    split_url(baseURL);
}

void URL::split_url(const std::string& url)
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

std::string URL::resolve_url(const std::string &url)
{
    //This is for url start which has :// whoose are absolute before like https://www.xyz.com/abc
    //return them as they are 
    for(int i = 0;i+2 < url.size(); ++i)
    {
        if(url[i] == ':' && url[i+1] == '/' && url[i+2] == '/'){
            return url;
        }
    }

    //if the url is not absolute and are like //abc.example these are protocol relative
    //add with schme like https + '//:' and the url
    if(url.size() > 2 && url[0] == '/' && url[1] == '/')
    {
        return scheme + ":" + url;
    }

    //if the url are like /about they are root relative
    //add them as path after the host
    if(url.size() > 1 && url[0] == '/' && url[1] != '/')
    {
        return scheme + "://" + host + url;
    }

    //now this is for cases like url just as "contact" or "..contact" these are just relative
    //must be placed after the current path of baseurl
    std::string basepath = path;
    if(basepath.empty())
    {
        basepath = '/';
    }
    int last_slash = -1;
    for(int i = basepath.size()-1; i >= 0; --i)
    {
        if(basepath[i] == '/')
        {
            last_slash = i;
            break;
        }
    }
    std::string toadd_bef_ru = "";
    for(int i = 0;i <= last_slash;++i)
    {
        toadd_bef_ru += basepath[i];
    }

    return scheme +"://" + host + toadd_bef_ru + url;
}