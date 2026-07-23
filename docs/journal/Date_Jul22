# Day 4 Journal

## Section 1 — Specific Bug

### Bug 1: Empty HTML Pages

While testing the crawler, I noticed that although many URLs were being visited, most of the downloaded HTML pages were empty. I suspected two possible causes:

- The fetcher was not downloading the pages correctly.
- The URL normalization logic was producing invalid URLs.

### Bug 2: URL Components Appended Repeatedly

While debugging the URL handler, I found that calling `split_url()` multiple times on the same `URL` object appended the parsed components to the existing member variables instead of replacing them.

### Bug 3: Redirected URLs Not Fetched

Many websites redirected HTTP requests to HTTPS using a **301 Moved Permanently** response. Since libcurl was not configured to follow redirects, the fetcher was receiving the redirect response instead of the webpage.

### Bug 4: Successful Fetch with Empty HTML

I discovered that `curl_easy_perform()` returned `CURLE_OK` even when the downloaded HTML string was empty. As a result, the crawler treated empty pages as successfully downloaded.

### Bug 5: Incorrect URL Resolution

While testing the crawler, I found that my URL resolver did not preserve the port number when resolving URLs, producing incorrect absolute URLs.

### Bug 6: Incorrect Normalization

The normalization function also failed to correctly handle several URLs containing `../`, causing invalid normalized URLs to be generated.

---

## Section 2 — Failed Attempt

My initial assumption was that the fetcher was returning empty pages because requests were being sent too quickly. After reviewing the fetcher implementation, I realized that the real problem was more complex.

First, I discovered that the fetcher did not automatically follow HTTP redirects, so many requests never reached the final webpage. I enabled `CURLOPT_FOLLOWLOCATION` to solve this.

Next, I noticed that even when libcurl reported `CURLE_OK`, the downloaded HTML could still be an empty string. Treating every `CURLE_OK` response as a successful fetch was therefore incorrect. I modified the fetcher so that an empty response is treated as a failed fetch.

While testing again, I realized that the link extractor also needed improvement. My original parser only worked for simple HTML such as `<a href="...">`. It failed to handle links containing additional spaces, uppercase HTML tags, links without quotation marks, and malformed HTML. I rewrote the extraction loop to support these different formats.

Finally, I discovered that several URL normalization bugs were also contributing to the problem. The resolver was not preserving port numbers, and the normalizer incorrectly handled relative paths containing `../`. Fixing these issues greatly improved the crawler's reliability.

---

## Section 3 — Code Reference

**Commit Hash**

`d92f957 fix link extractor and fetcher problem`

`a2c645c fix: correct url resolver and normalizer logic`

**Files Modified**

- `include/frontier.h`
- `include/fetcher.h`
- `src/fetcher.cpp`
- `include/url_handler.h`
- `src/url_handler.cpp`
- `src/webcrawler.cpp`
- `include/page.h`
- `src/page.cpp`
- `include/database.h`
- `src/database.cpp`

---

## Section 4 — Learning Reflection

Today I learned that building a reliable web crawler requires much more than simply downloading web pages. Modern websites frequently redirect users from HTTP to HTTPS, and unless the fetcher is configured to follow these redirects, many requests will never reach the intended destination. I also learned that a successful network request does not necessarily mean useful data was received. Even when `curl_easy_perform()` returns `CURLE_OK`, the response body may still be empty, so additional validation is necessary before considering the fetch successful.

Another important lesson was about designing robust parsers. Initially, I assumed that most hyperlinks would appear in a single HTML format, but real webpages use many different variations of the `href` attribute. Supporting whitespace, uppercase tags, missing quotation marks, and malformed HTML required me to redesign the extraction logic rather than continue patching the original implementation.

Finally, I began thinking about the storage architecture of the crawler. Instead of writing pages directly to disk during crawling, I designed a `Page` class to represent downloaded pages in memory and started integrating SQLite as a storage backend. Separating the crawler, page representation, and database storage made me realize how dividing responsibilities between components results in a cleaner and more maintainable system.