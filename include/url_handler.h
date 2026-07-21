#ifndef URL_H
#define URL_H

#include <string>

class URL
{
    private:
    std::string scheme;
    std::string host;
    std::string port;
    std::string path;
    std::string query;
    std::string fragment;

    public:

    URL(const std::string& baseURL);
    std::string resolve_url(const std::string &url);
    std::string normalize_url(const std::string &url);
    void split_url(const std::string& url);

};

#endif