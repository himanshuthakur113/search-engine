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