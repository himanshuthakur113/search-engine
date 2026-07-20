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