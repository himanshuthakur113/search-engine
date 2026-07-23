# Build Log

## Date: 22/07/2026

### Session - 1

### Work Completed
- Continued testing the crawler after integrating the fetcher, URL handler and link extractor.
- Noticed that most of the downloaded pages were empty, indicating a problem somewhere in the crawling pipeline.
- Started debugging by identifying the possible reasons behind the issue instead of directly changing the code.

### Problems Faced
Most of the crawled pages were completely empty.


I considered two possible reasons for this:

- The `Fetcher` might be returning an empty HTML page because requests are being sent too quickly and the page is not fetched correctly.
- The URL normalization logic might be generating incorrect URLs, causing invalid requests to be sent to the server.

### Solution
I decided to first verify the URL handling logic before making changes to the fetcher, since an incorrect URL would always result in an incorrect request.

While reviewing the `URL` class, I found a bug in the `split_url()` function.

The URL components (`scheme`, `host`, `port`, `path`, etc.) are stored as member variables of the `URL` object. If `split_url()` is called more than once using the same object, the new components are appended to the existing values instead of replacing them.

Although this bug was not affecting the current implementation, it could easily introduce unexpected behavior later.

I fixed it by clearing all URL component members at the beginning of the `split_url()` function before parsing a new URL.

---

## Date: 22/07/2026

### Session - 2

### Work Completed
- Improved the `Fetcher` implementation to handle common HTTP scenarios more reliably.
- Added additional libcurl options to improve crawling stability.

### Changes Made
I added support for the following libcurl options:

- `CURLOPT_FOLLOWLOCATION` to automatically follow HTTP redirects (301/302). Previously, if an HTTP URL redirected to HTTPS, libcurl stopped at the redirect and returned an empty page.
- `CURLOPT_TIMEOUT` to limit the maximum time allowed for the entire HTTP transfer.
- `CURLOPT_CONNECTTIMEOUT` to limit the time spent establishing a connection.
- `CURLOPT_USERAGENT` to send a browser-like User-Agent instead of the default `libcurl/x.x.x`, since some servers block requests made by the default libcurl user agent.

While debugging, I also noticed another issue.

`curl_easy_perform()` was returning `CURLE_OK` even when no HTML content had actually been written to the response buffer.

I modified the fetch logic so that an empty HTML response is treated as a failed fetch instead of a successful one.

---

## Date: 22/07/2026

### Session - 3

### Work Completed
- Continued improving the URL extraction logic.
- Rewrote the extraction loop to correctly handle several HTML variations.

### Problems Faced
The previous implementation assumed that links always followed a fixed format such as:

```html
<a href="...">
```

However, many real web pages contain links in formats like:

```html
<a      href = "...">
```

```html
<A HREF="...">
```

```html
<a href = www.example.com>
```

or even malformed HTML like:

```html
<a href = "abc.com>
```

The previous parser was unable to correctly handle these cases.

### Solution
I rewrote the entire extraction loop to make it much more robust.

The new implementation now handles:

- Multiple spaces between attributes.
- Uppercase HTML tags and attributes.
- Quoted and unquoted URLs.
- Malformed HTML whenever possible.
- Different attribute formatting styles.

This makes the extractor significantly more reliable when crawling real-world websites.

---

## Date: 22/07/2026

### Session - 4

### Work Completed
- Continued debugging the URL normalization logic.
- Fixed additional bugs in URL resolution.

### Problems Faced
While testing different URLs, I found another issue in the `resolve()` function.

The resolved URL was missing the port number whenever the original URL contained one.

I also found several normalization bugs while handling relative paths containing:

```text
../
```

These cases were producing incorrect normalized URLs.

### Solution
- Fixed the missing port while resolving URLs.
- Corrected the normalization logic for relative paths.
- Verified that URLs containing `../` are now resolved correctly.


---

## Date: 22/07/2026

### Session - 5

### Work Completed
- Started redesigning the storage system for crawled pages.
- Decided to separate page storage from the crawler logic.

### Design Decision
Instead of writing every downloaded page directly to disk during crawling, I decided to introduce an intermediate storage layer.

The new design works as follows:

- Store crawled pages temporarily in memory.
- After crawling is complete, flush all pages to permanent storage.
- Initially the pages will be written to disk, but the design also allows replacing the storage backend with a database later.

To support this design I created a `Page` class that stores:

- Page ID
- URL
- HTML content

I also designed a `PageStore` class responsible for buffering pages and managing storage independently of the crawler.


After completing the buffering logic, I started integrating SQLite as the storage backend.

I successfully connected the project to an SQLite database and implemented the initial storage layer. Currently, each crawled page is stored with the following fields:

- `id`
- `url`
- `html`

This design cleanly separates crawling from storage and makes it easier to extend the project with indexing and search functionality later.