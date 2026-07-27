#include "Indexer/posting_list.h"

void PostingList::add_posting(int pageId)
{
    if(contains(pageId))
    {
        return;
    }

    Posting temp;
    temp.page_id = pageId;
    postings.append(temp);
}

bool PostingList::contains(int pageId) const
{
    for(int i = 0;i < postings.size(); ++i)
    {
        if(postings[i].page_id == pageId)
        {
            return true;
        }
    }
      return false;
}

const DA<Posting>& PostingList::getPostings() const
{
 return postings;   
}