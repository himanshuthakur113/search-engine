#include <string>
#include <curl/curl.h>

class Fetcher
{
    private:
        static size_t WriteCallback(char* data, size_t elementSize, size_t no_of_bytes, void* userdata);
    public:
        Fetcher();
        ~Fetcher();
        std::string fetch(const std::string& url);
};