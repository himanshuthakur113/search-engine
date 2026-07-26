#ifndef POSTING_LIST_H
#define POSTING_LIST_H

#include "posting.h"
#include "dynamic_array.h"
class PostingList
{
    public:

        void add_posting(int pageId);
        bool contains(int pageId) const;
        const DA<Posting>& getPostings() const;

    private:

        DA<Posting> postings;
};

#endif