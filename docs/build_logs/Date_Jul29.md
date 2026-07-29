first the issue with the stored keyword they are like single char 
![single char](image-32.png)

also the second issue is with only number keyword
![only number](image-33.png)

fixing these two problems by 
not returning word which has less letter than two char
if the word does not contain any alphabet than does not include it


after solving it lets move to testing but first I am going to test the while pipeline from start like from crawler to storing pages and then making index
ok the new test with different url is successful
the result are better then previous 
![alt text](image-34.png)

now next going to move with g-test testing 
first i test the html parser 
here 3 test fails
![alt text](image-35.png)

MixedHTML
IgnoreStyle
IgnoreScript

this is happening because
IgnoreScript

result = ""
Expected = "Hello"

The HTML is:

<script>int x = 10;</script><body>Hello</body>

An empty string means once the parser entered SCRIPT mode, it never came back to TEXT mode.

IgnoreStyle

result = ""
Expected = "Hello"

Same behavior.

MixedHTML

result = "Demo"
Expected = "Demo Hello World"

It successfully parsed the <title>Demo</title>, then encountered <style> and never processed anything after it.

The problem

Your function is designed to match opening tags, not closing tags.

Look at the last part:

char next = html[index + tag.size()];
return next == '>' || std::isspace(next);

Now consider this call:

match_tag(html, i, "</script>")

The tag string already contains the >.

Suppose the HTML is

</script><body>

After matching "</script>", next becomes:

<

because you've already consumed the >.

Then this line returns

return '<' == '>' || isspace('<');

which is false.

recorrecting the error
fixing by replacing the match tag fxn for </script> and </style> by html.compare
here it passes all the test cases 
![alt text](image-36.png)

now testing tokenizer failed one test case that is the alphanumeric
![alt text](image-37.png)
ok the problem is that my tokenizer only take alphanumeric character as token not like any symbols 

changing the test case as I only want the toknzier to return word not word with symbols 
after it all the test cases pass
![alt text](image-38.png)

next i test normalizer and it passes all test cases
![alt text](image-39.png)