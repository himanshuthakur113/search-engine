I start working on left test cases 
first i working on fetcher 

okay one of the fetcher test case failed 
FetcherTest.ReturnedContentContainsHTML

![Fetcher failed](image-21.png)

Problem can'nt be solved moving next

Now I am testing extractlink function the html provided to it contains different links then going to test that

It passes all the test cases
![extract_links](image-22.png)

Now moving to frontier test cases 
it Passes all the test cases
![frontier_test](image-23.png)

After it the visitedSet test 
it pases all the test cases
![visitedSet](image-24.png)

Now fixing the project bugs 
one bug that I encountered is that the buffer can have two same pages and at flush it can also put duplicate pages in the database and it will throw 
db cannot insert page error

Fix add a hashmap to the pagestore to check if the page is already inside buffer 

Also since the pages are stored in data buffer if they are alot they can take a lot of ram space therefore flushing pages in ram after 10 pages 

The flush transaction before is just using insert each would have a separate disk write therefore i added begin commit and rollback so transaction would take one disk write
increasing the speed 


okay now i am working on Indexer 
first it do html parsing 
in this only the content from the html is extracted
not the tags what the content the user view
 
 I am going to implemented the indexer inside the crawler folder i have created two seprate folder one in include and one in src name indexer

 have implemented a simple extract text function inside the html parser class(git commit here )

Now i am going to implement simple extract logic for `<script>` or `<style>` type of tags the content of whose I do not need

Now rather than using boolean value for state I am going to use enum class 
what is enum class -> enum class is a user defined type which represent a fixed set of named value instead of arbitary integer

like enum ParserState
{
    TEXT,
    TAG,
    SCRIPT,
    STYLE,
}

it is roughly equivalent to TEXT = 0,TAG =1
why used this if(state == 1) if we can do state == TAG show the intent clearly 
i use switch statement to ignore `<script> and <style>`

I still does not handle yet <SCRIPT> type of tags
after it i implemented the comments by looking of comment and updating the state

Now I am going to work on next function going to handle other html_parser cases in the fututre

Next I am going to work on tokenizer 
okay the tokenizer works fine now I am going to work on normalizer 

What normalizer do it do lot of stuff 
but for right now I am making it to do only lowercasing 

ok after it lets build the index data structure
posting is the smallest unit of return inverted index
for now it will just contain just the page_id

the page_id it will use is from the database 

next will be to implement a PostingList class the posting list class will have a dynamic array which will contain the Posting structure which contain the page_id

after it I implement the inverted_index.h for which uses hashmap as a private member 
for two fxn of inverted_indexer  i need to make change in the hash_map 
first i need to make the class member function contains const which is good thing so that it will not modify any class member ,then there is find function which return a V* but i need to overload it to return const V* for implementation of search and contains
while adding const to hash_map i notice that it uses linkedlist find which also does not have const return so i need to add a overload to linkedlist too.

after it i implemented the indexer.h and indexer.cpp where i added the entire workflow for the indexer
It is like 
Page

↓

HTMLParser

↓

Plain Text

↓

Tokenizer

↓

DA<string>

↓

Normalizer

↓

InvertedIndex

↓

HashMap

↓

PostingList

↓

Posting

After it i make some changes in the crawler as in the crawler pagestore has function name pages_saved which is returning the number of pages saved in the buffer not in the actual database so I have decided to create another function which return the actual total number of pages saved and renamed this pages_saved to buffered_page()

then i need to make some changes in the crawler crawl function added a processed page data member to Webcrawler class which will tell total number of pages processed 
also refactor some crawler processing pipeline

Frontier
    │
    ▼
Pop URL
    │
    ▼
Check Cache
    │
 ┌──┴────────┐
 │           │
 ▼           ▼
SQLite    Network
 │           │
 └────┬──────┘
      ▼
 Build Page
      │
 ┌────┴─────┐
 ▼          ▼
Save      Extract Links
(if new)
      │
      ▼
Resolve URLs
      │
      ▼
Visited Check
      │
      ▼
Push to Frontier
      │
      ▼
processed_pages++