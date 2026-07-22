![alt text](image-10.png)
![alt text](image-11.png)
there is an issue the most of the pages i am getting are empty 
blank pages are coming because
there might be two reason 
one can be fetching function would return empty pages as the request are coming fast and it will not be able to fetch the page fast so it return a empty page

second can be my url normalization function is giving wrong url

so first I am going to work with my url handles 
there's a bug that I notice which is that the 
url handler object has the url component as member but if a call the spilit url function again using the object it will append new url components to the object member although it does not count any problem it is important to clear the old member if this function called simple fix is to clear all the member in the starting.

okay I have made the changes in the fetcher.cpp 
the problem that I maybe faces is that if the http link redirect to https link the server give code 301 for this the libcurl will stop and return empty page therefore I have added another option name as FOllOWLOCATION with parameter 1 which mean true activate it

TIMEOUT if the http transfer didn't have in the given time then return empty

CONNECTIONTIMEOUT for establishing a session there is a time limit 

USERAGENT pass a name to the server not libcurl.x.x.x some browser might block libcurl

The problem acutally is that the in the fetcher the perform function is returning curl_ok even if the return html is empty string but now it would not return curle_ok if it didn't write the string.

