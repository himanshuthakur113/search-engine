# Design Proposal - Web Crawler

---

# Objective

The objective of this project is to build a Web Crawler that starts from one or more seed URLs. It downloads the HTML of each page and extracts the links from that page to discover new pages to visit. It contains a Frontier to manage URLs waiting to be crawled and a Visited Set to avoid revisiting URLs that have already been visited. Every crawled page is stored with its HTML content so it can be used by the Indexer to build a searchable index. This project demonstrates how early search engines collected web pages before users searched them, like Google's search engine in 1998.

---

# Section 1 — Public API

## 1. Fetcher

```cpp
class Fetcher
{
public:
    std::string fetch(const std::string& url);
};
```

## 2. Frontier

```cpp
struct CrawlTask
{
    std::string url;
    int depth;
};

class Frontier
{
public:
    void push(const CrawlTask& task);
    CrawlTask pop();
    bool empty() const;
    int size() const;
};
```

## 3. Visited

```cpp
class Visited
{
public:
    bool contains(const std::string& url) const;
    void markVisited(const std::string& url);
    int size() const;
};
```

## 4. PageStore

```cpp
class PageStore
{
public:
    void storePage(const std::string& url,
                   const std::string& html,
                   int depth);

    std::string getPage(const std::string& url);

    bool hasPage(const std::string& url);

    int pageCount() const;
};
```

## 5. WebCrawler

```cpp
class WebCrawler
{
public:
    WebCrawler(int maxDepth, int maxPages);

    void crawl(const std::string& seedURL);
};
```

---

# Section 2 — Complexity Estimates

| Operation | Best | Average | Worst | Why |
|-----------|------|---------|-------|-----|
| `fetch(url)` | O(H) | O(H) | O(H) | The fetch url will download the whole html therefore it need to  traverse the whole html file of size H. There also be time taken by the network to connect and transfer file. |
| `Frontier::push()` | O(1) | O(1) | O(1) | Inserts a `CrawlTask` at the rear of the queue. |
| `Frontier::pop()` | O(1) | O(1) | O(1) | Removes the front task from the queue. |
| `Frontier::empty()` | O(1) | O(1) | O(1) | Returns whether the queue is empty. |
| `Frontier::size()` | O(1) | O(1) | O(1) | Returns the maintained size counter. |
| `Visited::contains()` | O(1) | O(1) | O(n) | Average case performs a HashMap lookup. Worst case occurs when many URLs collide into the same bucket. |
| `Visited::markVisited()` | O(1) | O(1) | O(n) | Average case inserts into the HashMap in constant time. Worst case depends on bucket collisions. |
| `Visited::size()` | O(1) | O(1) | O(1) | Returns the maintained counter. |
| `normalize(url)` | O(L) | O(L) | O(L) | Each normalization step scans at most the entire URL once. |
| `extractLinks(html)` | O(H) | O(H) | O(H) | Scans the HTML once to extract every `href` attribute using a regular expression. |
| `storePage()` | O(H) | O(H) | O(H) | Writes the downloaded HTML to the PageStore. |
| `getPage()` | O(H) | O(H) | O(H) | Reads the stored HTML from the PageStore. |
| `hasPage()` | O(1) | O(1) | O(n) | Uses a HashMap lookup to determine whether a page exists. |
| `pageCount()` | O(1) | O(1) | O(1) | Returns the maintained page counter. |
| `crawl(seedURL)` | O(P * (H + N * L)) | O(P * (H + N * L)) | O(P * (H + N * V)) | For each of the P pages, the crawler downloads and scans the HTML in O(H) time. It then processes N extracted URLs, where each URL is normalized in O(L) time and checked against the Visited HashMap. HashMap lookup is O(1) on average but may degrade to O(V) in the worst case due to collisions, giving an overall complexity of O(P * (H + N * L)) on average and O(P * (H + N * L * V)) in the worst case |

### Notation

Where,

- **P** = Number of pages crawled.
- **H** = Average size of an HTML page.
- **N** = Average number of links extracted from a page.
- **L** = Average length of a URL.
- **V** = Number of visited URLs.

---
# Section 3 - Internal Representation

### Crawler Flow Diagram

![Crawler Flow Diagram](/Project_2/Crawler_HT/docs/design/memory_diagram/Crawler_flow.jpeg)

### Crawler Memory Diagram

![Crawler Memory Diagram](/Project_2/Crawler_HT/docs/design/memory_diagram/Crawler_memdiagram.jpeg)

---
# Section 4 — Design Decisions

## Frontier Data Structure

The Frontier is used to store the URLs that are waiting to be crawled. It is implemented using a Queue, which follows the FIFO (First In First Out) principle. The first URL inserted into the Frontier is the first one to be crawled. Using a Queue naturally performs a Breadth First Search (BFS) traversal of the web graph.

Although a Stack could also be used as the Frontier, it would perform a Depth First Search (DFS). BFS is preferred because it visits pages closer to the seed URL first, provides better coverage of the website, and avoids spending excessive time following a single deep path before exploring other pages.

## Visited Set

A HashMap is used to store all URLs that have already been crawled. Before visiting a page, the crawler first checks whether the URL already exists in the Visited Set. If the URL has already been visited, it is skipped. Otherwise, it is inserted into the Visited Set and processed.

A normal array or linked list could also be used, but searching for a URL would require **O(n)** time. Using a HashMap reduces the average lookup time to **O(1)**, making duplicate detection much more efficient as the crawler grows.

## URL Normalization

Before storing or checking a URL in the Visited Set, the URL is normalized so that different representations of the same webpage are treated as a single URL. Without normalization, the crawler may visit and store the same page multiple times.

The normalization process performs the following steps:

- **Convert the scheme and host to lowercase.**

```text
https://Example.com/About
https://example.com/about
```

Both URLs refer to the same page, but without normalization they are treated as different strings.

- **Remove trailing slashes from non-root paths.**

```text
https://example.com/about
https://example.com/about/
```

These usually refer to the same webpage.

- **Remove fragment identifiers (`#`).**

```text
https://example.com/page#section1
https://example.com/page#section2
https://example.com/page
```

Fragment identifiers only tell the browser where to scroll within the page. They are never sent to the web server, so all three URLs represent the same webpage.

- **Remove empty query parameters.**

```text
https://example.com/page
https://example.com/page?
```

Both URLs represent the same webpage.

- **Remove default ports.**

```text
http://example.com
http://example.com:80
```

Port **80** is the default HTTP port, making these URLs equivalent.

After normalization, every equivalent URL is converted into a single canonical representation before it is inserted into or searched in the Visited Set. This prevents duplicate crawling, duplicate storage, and unnecessary network requests.

## Link Extraction Logic

Links inside an HTML document are stored in the `href` attribute of anchor (`<a>`) tags. To discover new pages, a regular expression (regex) is used to locate every `href` attribute and extract its value.It will return a array of url.

After extraction:

- Relative URLs are resolved into absolute URLs using the current page's URL.
- URLs beginning with `#` are ignored because they only reference another section of the current page.
- URLs beginning with `javascript:` are ignored because they do not represent actual webpages.
- Every extracted URL is passed through the URL Normalization component.
- The normalized URL is checked against the Visited Set.
- If the URL has not been visited before and the depth limit has not been exceeded, it is inserted into the Frontier.

## Fetch Failure Handling

If `fetch()` returns an empty string, the crawler assumes that the page could not be downloaded. The page is skipped, and the crawler continues processing the next URL in the Frontier.

## Page Storage

Every successfully crawled webpage is stored in a single structured file. Each record contains the webpage URL, crawl depth, and HTML content. The stored pages are later used by the Indexer to build a searchable index.

## Crawler Termination

The crawler stops under either of the following conditions:

- The number of crawled pages reaches `maxPages`.
- The Frontier becomes empty, indicating that there are no more pages to visit.

## Depth Limit

When links are extracted from a webpage, the crawler calculates the depth of each discovered page.

If:

```text
currentDepth + 1 > maxDepth
```

the URL is discarded and is not inserted into the Frontier.

The default value of `maxDepth` is **3**.

## Crawl Delay

Sending too many requests to a web server within a short period of time may overload the server or cause the crawler to be blocked. To reduce server load and behave more responsibly, a delay of **1000 ms (1 second)** is introduced before every fetch request.