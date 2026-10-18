#ifndef POSTING_LIST_H
#define POSTING_LIST_H

#include "posting.h"
#include "dynamic_array.h"
class PostingList
{
    public:

        // Adds a page to the list. If the page is already there, its count
        // grows by `count` instead of adding a second posting.
        void add_posting(int pageId, int count = 1);
        // How many times the word appears on the page (0 if it does not)
        int count_for(int pageId) const;
        bool contains(int pageId) const;
        const DA<Posting>& getPostings() const;

    private:

        DA<Posting> postings;
};

#endif