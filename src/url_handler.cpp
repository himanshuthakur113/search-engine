#include "url_handler.h"
#include "dynamic_array.h"

URL::URL(const std::string& baseURL)
{
    split_url(baseURL);
}

void URL::split_url(const std::string& url)
{
    scheme.clear();
    host.clear();
    port.clear();
    path.clear();
    query.clear();
    fragment.clear();

    int i = 0;

    //for scheme
    while(i + 2 < url.size())
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
        //here i haven't been incremented because path need to capture the first / 
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

    std::string host_port = host;
    if(!port.empty())
    {
        host_port += ":" + port;
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
        return scheme + "://" + host_port + url;
    }

    //now this is for cases like url just as "contact" or "..contact" these are just relative
    //this does not remove ../
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

    return  scheme +"://" + host_port + toadd_bef_ru + url;
}

std::string URL::normalize_url(const std::string &url)
{
    URL temp(url);

    //if the created url didn't have path add /
    if(temp.path.empty())
    {
        temp.path = '/';
    }

    for(int i = 0;i < temp.scheme.size(); ++i)
    {
        temp.scheme[i] = tolower(temp.scheme[i]);
    }

    for(int i = 0;i < temp.host.size(); ++i)
    {
        temp.host[i] = tolower(temp.host[i]);
    }

    if((temp.port == "80"&& temp.scheme == "http")||(temp.port == "443"&& temp.scheme == "https"))
    {
        temp.port = "";
    }

    temp.fragment = "";

    //this is for .. and . segments 
    std::string dotdotpath = temp.path;
    std::string path_part;
    DA<std::string> path_parts;
    int top = -1;
    for(int i = 0;i < dotdotpath.size(); ++i)
    {
        if(dotdotpath[i] == '/')
        {
            if(path_part == ".."){
                if(top >= 0) top--;
                path_part.clear();
            }
            else if(path_part != "." && path_part != "")
            {   
                top += 1;
                path_parts[top] = path_part;
                path_part = "";
            }
            else
            {
                path_part = "";
            }   
        }
        else
        {
            path_part += dotdotpath[i];
        }
    }

    if(path_part == "..")
    {
        if(top >= 0)
        {
            top--;
        }
    }
    else if(path_part != "." && path_part != "")
    {
        top++;
        path_parts[top] = path_part;
        path_part = "";
    }

    std::string combined_path;
    for(int i = 0;i <= top; ++i)
    {
        combined_path += "/" + path_parts[i];
    }

    if(combined_path.empty())
    {
        combined_path = "/";
    }

    temp.path = combined_path;

    if(temp.port != "")
    {
        temp.port = ":" + temp.port;
    }

    if(temp.query != "")
    {
        temp.query = "?" + temp.query;
    }

    return temp.scheme + "://" + temp.host + temp.port + temp.path + temp.query;
}