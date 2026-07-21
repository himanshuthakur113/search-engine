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

    URL(const std::string& url);
    std::string resolve_url(const std::string baseUrl);
    std::string normalize_url(const std::string url);

};