#include "fetcher.h"
#include <iostream>

Fetcher::Fetcher()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

Fetcher::~Fetcher()
{
    curl_global_cleanup();
}

size_t Fetcher::WriteCallback(char* data, size_t elementSize, size_t no_of_bytes, void* userdata)
 {  
    
    std::string* html = static_cast<std::string*>(userdata);
    html->append(data, elementSize * no_of_bytes);
    return elementSize * no_of_bytes;
 }

std::string Fetcher::fetch(const std::string& url)
{
    CURL* handle = curl_easy_init();
    std::string html;

    if(!handle)
    {
        throw std::runtime_error("can not make the handle");
    }

    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA,&html);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "ht");

    CURLcode res = curl_easy_perform(handle);

    if(res != CURLE_OK)
    {   
        std::string err_msg = curl_easy_strerror(res);
        curl_easy_cleanup(handle);
        throw std::runtime_error("fetch failed: " + err_msg);
    }
    curl_easy_cleanup(handle);

    return html;
}