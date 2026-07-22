#include <string>

class PageStore
{
    private:
        std::string directory;
        int page_cnt;

    public:
        PageStore(const std::string &dir = "../pages");
        void save(const std::string &html);
        int pages_saved() const;

};