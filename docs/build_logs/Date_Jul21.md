I start by resuming the url_handler instead of spliting the string into different component in the constructor I write a differnet function name split_url which spilit the url into different components.Then I will call this function inside the constructor.
I have implemented the resolver function this function will take two arguments one is base url and the second is the url which is going to be resolved.I have added 4 different cases 
if the url is absolute then return it 
if the url is protocol relative then modify and return it 
if the url is root relative then add it after the url and return it
if the url is relative like simple "contact" ..contact
then add it after url base then return it

Now I am adding this to crawl function of WebCrawler, there is something here which I notice right now that is I have to initialize the URL object in the beginning when I pop the URL from frontier and pass it the the url but that is going to be base URL and it is a good thing at this have the base URL components it will easy for comparision in the resolver function.
I have make the changes and going to add the resolve_url to the crawl function and running it works fine.