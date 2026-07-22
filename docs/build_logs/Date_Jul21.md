I start by resuming the url_handler instead of spliting the string into different component in the constructor I write a differnet function name split_url which spilit the url into different components.Then I will call this function inside the constructor.
I have implemented the resolver function this function will take two arguments one is base url and the second is the url which is going to be resolved.I have added 4 different cases 
if the url is absolute then return it 
if the url is protocol relative then modify and return it 
if the url is root relative then add it after the url and return it
if the url is relative like simple "contact" ..contact
then add it after url base then return it

Now I am adding this to crawl function of WebCrawler, there is something here which I notice right now that is I have to initialize the URL object in the beginning when I pop the URL from frontier and pass it the the url but that is going to be base URL and it is a good thing at this have the base URL components it will easy for comparision in the resolver function.
I have make the changes and going to add the resolve_url to the crawl function and running it works fine.

Writing the url normalizer function in this I have created a temp object of url class which is going to be have component of the url extracted form the page

then I normalize the url given by resolver function 
I fix the path in path if empty there always /
covert to lower case the scheme and the host
remove fragment
handle ../ these paths
handle port  ->remove default port


after that I integreated it in the crawl function and run it I faces this error
![alt text](image-8.png)
In this error the important par tis operator[](size_type) it is from basic_string.h
and error is _pos <= size() failed

It means I am accessing a character outside the bounds of std::string

Okay it is inside my link extractor function in link extractor I have changed something one condition in the loop which is the loop will run upto html.size() - 7 which is wrong
NO it is not the issue loop will not crash I am not accessing element above the html.size()-1

The issue can be in split_url part where I try to access url[i] for getting schemes but it is not the issue as I face the similar error again 

Now I decided to debugging I run the main using gdb and this is the line where the actual error occur

![errorx](image-9.png)

The error is caused when the html.size() = 0 and I try to run the loop until html.size() - 6.html.size() will return a size_t which can'nt be negative so it becomes the biggest int causing segmentation fault
I fix this by converting it to i + 6 <   html.size()

