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