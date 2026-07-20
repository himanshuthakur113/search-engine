Today, I have started working on extraction logic of url from the html page returned from the fetch function.
First I created a private function name link_extractor inside the WebCrawler Class,
I am thinking it should return a array of url extracted from the html file then we process each url individually.
I have written a temporary url finding logic inside the function it is just a simple hardcode logic inside the loop.

While testing the simple url extractor logic which I defined in the WebCrawler class I need to make its object in the main,
 while building the main I realizes that my data structures from the main library are not included 
 ![alt text](image-3.png)

 To solve this I think of adding the ds library as a subdirectory inside the webcrawler I made changes in both CMakeLists.txt of ds library and Crawler.Then I face another issue which is 
 ![alt text](image-4.png)

 It happens because the compiler tries to find the linked_list.tpp which is used in th hashmap inside the include folder which is inside the src folder.This is issue is fixed by making some changes in the CMake of the ds-library adding src as an include directory to interface library.

After this I encounter another bug which is ![alt text](image-5.png)
It is error in my Dynamic array shrink function I have used a ptr but haven't declared it
Problem -> The problem is I have declared a ptr inside a if block and and has used it outside the block 
Solution -> I have decided to create it outside the if block intializing it with nullptr

Finally the link extractor work it give everything after the (<)a href =  until (>) this is not found.

--------
Now I am continue working on the same url extraction logic,made some changes and now it is not printing anything 
Okay I have fixed the issue 
To understand the problem first its important how my code works there is a for loop which traverse until it found < a and ' ' after that it increment i by 3 then another while loop will run which will not stop until it found https now here I face the problem which is I created while loop like this 
while(html[i] != 'h' && html[i+1] != 't' && html[i+2] != 't' && html[i+3] != 'p' && html[i+4] != 's')
{   
    i++;
}
It is not right because this loop see just one occurence and if it does not match https it stops

then i replace my while loop like this 

while(html[i] != 'h' || html[i+1] != 't' || html[i+2] != 't' || html[i+3] != 'p' || html[i+4] != 's')
{   
              i++;
}

This loop will run until it found https and then stop after that.It then just copy the URL starting from the html[i] until the html[i] becomes equal to the start position which is intialize in the beginning of loop start = html[i-1]

This is just a example version it has alot of problem like it do only https not http and relative URL

I found a new idea going to find href, after href the loop will skip character until either " , ' are found.
Doing so I found about that for having ' inside '' I need to do it like '\''

eg -> <a href = "www.htw.com"> then first we look for (<)(a)( ) after that we increment ptr with 3 then if we find href then we increase ptr by 4 then we run while loop unitl we reach " then we start copy the arry from ptr + 1 position unitl we reach another ' or " and we add the copied url into the array.
At the last of function we return the array of url 

I face another problem it didn't give any result 
Problem - The problem is that i have written wrong its a typo mistake only I have write != rather than ==

The output is like this ![alt text](image-6.png)

It is not only returning URL it is returning 
simple # 
tel:
mailto:
https:
http:
javascript:void(0)

Now there is need to normalize thse url for which I am going to create a different normalize function

---------
Before making the normalizer I try to run the whole web crawler.I write a simple implementation in the crawl function and save the html in the page but there are come some issue to fix since queue implementation is generic I haven't added the #include "queue.tpp" at the end of queue.h file 

I corrected those mistake then run the process,at execution i encounter a error 
![alt text](image-7.png)
It is in the fetcher the fetcher is unable does not return res with CURLE_OK so means not able to fetch the page.
As per my understanding this is done because the url is not normalize and is not returning anything
I have written excpetion for this as if the curl_easy_perform fail now I am going to write try/catch block around the fetcher block 
Okay it is also not writing the pages in the page folder.
The problem is that the function has created the pages folder inside the build folder 