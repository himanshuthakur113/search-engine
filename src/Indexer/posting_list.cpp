#include "Indexer/posting_list.h"

void PostingList::add_posting(int pageId, int count)
{
    for(int i = 0;i < postings.size(); ++i)
    {
        if(postings[i].page_id == pageId)
        {
            postings[i].count += count;
            return;
        }
    }

    Posting temp;
    temp.page_id = pageId;
    temp.count = count;
    postings.append(temp);
}

int PostingList::count_for(int pageId) const
{
    for(int i = 0;i < postings.size(); ++i)
    {
        if(postings[i].page_id == pageId)
        {
            return postings[i].count;
        }
    }
    return 0;
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