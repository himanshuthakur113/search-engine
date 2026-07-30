# CRAWLER DESIGN PROPOSAL

**Project:** Web Search Engine – Crawler Module

---

# 1. Introduction

The crawler is the first stage of the search engine pipeline. Its responsibility is to discover web pages, download their HTML content, extract hyperlinks, and store the downloaded pages in a persistent database for use by the Indexer.

The crawler starts from a seed URL supplied by the user. Using a breadth-first traversal strategy, it continuously discovers new hyperlinks while preventing duplicate visits through URL normalization and a visited URL manager.

To improve efficiency, the crawler checks whether a page already exists in the local database before downloading it from the Internet. Newly downloaded pages are stored using the `PageStore`, while cached pages are retrieved directly from the database.

The crawler is implemented using a modular architecture where networking, frontier management, duplicate detection, URL processing, and persistent storage are separated into independent components.

---

# 2. Functional Requirements

The crawler shall:

* Accept a seed URL.
* Traverse pages using Breadth-First Search.
* Download HTML pages using HTTP.
* Extract hyperlinks from HTML.
* Resolve relative links into absolute URLs.
* Normalize URLs.
* Avoid duplicate crawling.
* Store pages persistently.
* Retrieve cached pages from the database.
* Flush buffered pages before termination.

---

# 3. Crawling Workflow

The crawler follows the sequence implemented in `crawl()`.

```text
Create Seed CrawlTask
        │
        ▼
Push into Frontier
        │
        ▼
Mark Seed as Visited
        │
        ▼
──────────────────────────────────────────
While Frontier is Not Empty
──────────────────────────────────────────
        │
        ▼
Check Page Limit
        │
        ▼
Pop CrawlTask
        │
        ▼
Check Database Cache
        │
   ┌────┴─────┐
   │          │
 Cached    Not Cached
   │          │
   ▼          ▼
Read HTML   Download HTML
from DB     using Fetcher
   │          │
   └────┬─────┘
        ▼
Create Page Object
        │
        ▼
Store Page (if newly downloaded)
        │
        ▼
Reached Maximum Depth?
        │
   ┌────┴─────┐
   │          │
 Yes         No
   │          │
   ▼          ▼
Next Task  Extract Links
               │
               ▼
         Resolve URL
               │
               ▼
        Normalize URL
               │
               ▼
       Already Visited?
          │        │
         Yes      No
          │        │
          ▼        ▼
       Ignore   Mark Visited
                     │
                     ▼
              Push to Frontier
                     │
                     ▼
             Continue Crawling
──────────────────────────────────────────
Frontier Empty
        │
        ▼
Flush PageStore Buffer
        │
        ▼
Finish
```

---

## Workflow Explanation

1. The crawler receives a seed URL.
2. The seed URL is inserted into the frontier.
3. The seed URL is immediately marked as visited.
4. The crawler repeatedly removes one URL from the frontier.
5. Before performing any network request, the crawler checks whether the page already exists in the database.
6. Cached pages are read directly from SQLite.
7. Otherwise, the Fetcher downloads the page.
8. Newly downloaded pages are stored through the PageStore.
9. If the maximum crawl depth has not been reached, hyperlinks are extracted.
10. Every extracted URL is resolved and normalized.
11. URLs that have already been visited are ignored.
12. New URLs are marked as visited and inserted into the frontier.
13. When crawling completes, the PageStore flushes any remaining buffered pages to the database.

---
Great. Let's continue with the **Module Design** section. This part is based on the structure of your implementation.

---

# 4. Module Design

The crawler follows a modular architecture where each class has a single, well-defined responsibility. The `WebCrawler` class coordinates the crawling process while delegating specialized tasks such as networking, duplicate detection, URL processing, and storage to dedicated modules.

---

# 4.1 WebCrawler

## Responsibility

The `WebCrawler` class is the central controller of the crawler. It coordinates the complete crawling process but delegates specialized operations to other modules.

It does **not** perform networking, storage, or URL parsing directly. Instead, it invokes the appropriate module whenever required.

---

## Data Members

| Member                | Description                       |
| --------------------- | --------------------------------- |
| `Fetcher fetcher`     | Downloads HTML pages.             |
| `Frontier frontier`   | Stores pending crawl tasks.       |
| `VisitedSet visited`  | Tracks previously visited URLs.   |
| `PageStore pagestore` | Stores and retrieves pages.       |
| `int maxDepth`        | Maximum crawl depth.              |
| `int maxPages`        | Maximum number of pages to crawl. |
| `int processed_pages` | Number of processed pages.        |

---

## Public Interface

```cpp
WebCrawler();

WebCrawler(int maxDepth,
           int maxPages);

void crawl(const std::string& seedUrl);

DynamicArray<std::string>
link_extractor(const std::string& html);
```

---

## Responsibilities

The crawler performs the following operations.

* Initializes crawl settings.
* Creates the seed crawl task.
* Controls the crawling loop.
* Invokes the Fetcher.
* Invokes the PageStore.
* Extracts hyperlinks.
* Resolves relative URLs.
* Normalizes URLs.
* Prevents duplicate crawling.
* Enforces crawl depth.
* Enforces crawl page limits.

---

## Internal Workflow

```text
Seed URL

↓

Create CrawlTask

↓

Frontier

↓

Fetcher / Cache

↓

PageStore

↓

Extract Links

↓

Resolve URLs

↓

Normalize URLs

↓

VisitedSet

↓

Frontier
```

---

# 4.2 Fetcher

## Responsibility

The `Fetcher` module is responsible for downloading HTML pages from the Internet.

It encapsulates all libcurl functionality, allowing the crawler to remain independent of networking details.

---

## Public Interface

```cpp
std::string fetch(const std::string& url);
```

---

## Responsibilities

* Configure HTTP requests.
* Download HTML.
* Handle network failures.
* Return page contents.

---

## Workflow

```text
URL

↓

libcurl

↓

HTTP GET

↓

HTML

↓

Return to WebCrawler
```

---

## Design Rationale

Separating networking into an independent module follows the **Single Responsibility Principle**. Future changes to the networking library can be made without modifying the crawler logic.

---

# 4.3 Frontier

## Responsibility

The `Frontier` manages URLs waiting to be crawled.

Instead of exposing a queue directly, the project encapsulates it inside a dedicated class.

---

## Stored Element

Each entry is represented as:

```cpp
struct CrawlTask
{
    std::string url;
    int depth;
};
```

---

## Public Operations

```cpp
push()

pop()

front()

empty()
```

---

## Workflow

```text
New URL

↓

Create CrawlTask

↓

Push

↓

Queue

↓

Pop

↓

Crawler
```

---

## Design Rationale

The `Frontier` hides the underlying queue implementation from the crawler. This abstraction improves maintainability and allows the frontier implementation to evolve without affecting other modules.

The queue naturally implements **Breadth-First Search (BFS)**, ensuring that pages are crawled level by level.

---

# 4.4 VisitedSet

## Responsibility

The `VisitedSet` prevents duplicate crawling.

Rather than allowing the crawler to manipulate a hash table directly, duplicate detection is encapsulated inside its own class.

---

## Public Operations

```cpp
contains()

markVisited()

clear()
```

---

## Workflow

```text
Normalize URL

↓

VisitedSet

↓

Already Exists?

↓

Yes → Ignore

No → Insert
```

---

## Design Rationale

Encapsulating duplicate detection inside a dedicated module improves modularity and isolates the underlying data structure from the crawler.

Average lookup time is **O(1)** because the implementation uses a hash table.

---

# 4.5 URL Processing

The crawler processes every extracted hyperlink before inserting it into the frontier.

URL processing consists of three steps:

* URL Parsing
* URL Resolution
* URL Normalization

---

## URL Parsing

The parser separates a URL into its individual components.

Example

```text
https://example.com:443/docs/index.html?x=1#top
```

↓

```text
Scheme   : https

Host     : example.com

Port     : 443

Path     : /docs/index.html

Query    : x=1

Fragment : top
```

---

## URL Resolution

Relative hyperlinks are converted into absolute URLs.

Example

```text
Base URL

https://example.com/docs/index.html

Relative Link

../about.html

↓

Resolved URL

https://example.com/about.html
```

---

## URL Normalization

Normalization converts different representations of the same resource into one canonical URL.

Example

Before

```text
HTTPS://Example.COM:443/index.html#
```

After

```text
https://example.com/index.html
```

Normalization reduces duplicate crawling and improves storage efficiency.

---

# 4.6 Link Extractor

## Responsibility

The `link_extractor()` function scans downloaded HTML pages and extracts hyperlinks from anchor tags.

Unlike some crawler designs, hyperlink extraction is implemented as a **member function of `WebCrawler`**, keeping this functionality closely integrated with the crawling process.

---

## Workflow

```text
HTML Document

↓

Scan Characters

↓

Locate <a>

↓

Locate href

↓

Extract URL

↓

Store URL

↓

Return URL List
```

---

## Responsibilities

* Scan HTML.
* Detect anchor tags.
* Locate `href` attributes.
* Extract hyperlinks.
* Return extracted URLs.

---

## Design Rationale

The extractor performs lightweight HTML scanning without relying on external parsing libraries. This keeps dependencies minimal while providing sufficient functionality for hyperlink discovery.
---

Excellent. Now we cover the storage subsystem. This section is based on your implementation (`page.cpp`, `page_store.cpp`, and SQLite integration).

---

# 5. Storage Design

The storage subsystem is responsible for persisting downloaded web pages. It consists of two primary components:

* **Page** – Represents a downloaded web page.
* **PageStore** – Manages caching, buffering, and persistent storage using SQLite.

Separating storage from crawling logic improves modularity and allows the crawler to focus solely on page discovery.

---

# 5.1 Page

## Responsibility

The `Page` class represents a single downloaded web page. It acts as a data container that stores all information required by the crawler and later by the Indexer.

---

## Stored Information

| Field  | Description               |
| ------ | ------------------------- |
| `id`   | Unique page identifier    |
| `url`  | Canonical URL of the page |
| `html` | Downloaded HTML document  |

---

## Purpose

Every successfully downloaded page is converted into a `Page` object before being stored. Using a dedicated class simplifies data transfer between the crawler, storage system, and indexer.

---

# 5.2 PageStore

## Responsibility

The `PageStore` manages persistent storage of downloaded pages.

It acts as an abstraction over SQLite, allowing the crawler to save and retrieve pages without directly interacting with SQL statements.

---

## Responsibilities

* Initialize SQLite database.
* Store downloaded pages.
* Retrieve cached pages.
* Prevent duplicate storage.
* Buffer newly downloaded pages.
* Flush buffered pages.
* Maintain page identifiers.

---

## Public Interface

```cpp
bool isCached(const std::string& url);

std::string fetch(const std::string& url);

void save(const Page& page);

void flush();
```

---

## Internal Components

```text
                 PageStore
                     │
     ┌───────────────┼────────────────┐
     │               │                │
     ▼               ▼                ▼
 Buffer        SQLite DB      Buffered URL Map
```

The PageStore contains three major components:

* An in-memory buffer for newly downloaded pages.
* An SQLite database for permanent storage.
* A hash table used to track buffered URLs and prevent duplicates before flushing.

---

# 5.3 Storage Workflow

The storage process follows the sequence below.

```text
Downloaded HTML
        │
        ▼
Create Page Object
        │
        ▼
PageStore::save()
        │
        ▼
Already Buffered?
        │
 ┌──────┴──────┐
 │             │
Yes           No
 │             │
 ▼             ▼
Ignore     Add to Buffer
                 │
                 ▼
        Buffer Full?
                 │
          ┌──────┴──────┐
          │             │
         No            Yes
          │             │
          ▼             ▼
      Continue      Flush Buffer
                         │
                         ▼
                    SQLite Database
```

---

## Buffering Strategy

Instead of writing every downloaded page directly to SQLite, newly downloaded pages are first placed into an in-memory buffer.

Only when the buffer reaches a predefined size—or when crawling completes—are all buffered pages written to the database in a single operation.

This reduces disk I/O and improves crawling performance.

---

# 5.4 Cache Lookup

Before downloading a page from the Internet, the crawler checks whether the page already exists in the local database.

Workflow:

```text
URL
 │
 ▼
PageStore::isCached()
 │
 ├───────────────┐
 │               │
Yes             No
 │               │
 ▼               ▼
Read HTML     Network Fetch
from SQLite   using Fetcher
```

This avoids unnecessary network requests and significantly reduces crawl time when revisiting previously stored pages.

---

# 5.5 Buffered Storage

The PageStore temporarily stores newly downloaded pages in memory before writing them to the database.

```text
Page 1
Page 2
Page 3
Page 4
...
Page N

↓

Memory Buffer

↓

Single Database Write
```

Compared to writing each page individually, buffered storage minimizes the number of database transactions and improves throughput.

---

# 5.6 Transaction Management

When buffered pages are written to SQLite, the PageStore performs the operation within a database transaction.

```text
Begin Transaction
        │
        ▼
Insert Page 1
        │
Insert Page 2
        │
Insert Page 3
        │
Insert ...
        │
        ▼
Commit Transaction
```

If an error occurs during insertion, the transaction is rolled back to preserve database consistency.

Using transactions ensures that either all buffered pages are stored successfully or none are, preventing partial writes.

---

# 5.7 SQLite Database Design

The crawler stores downloaded pages in an SQLite database.

## Pages Table

| Column | Type    | Description              |
| ------ | ------- | ------------------------ |
| `id`   | INTEGER | Unique page identifier   |
| `url`  | TEXT    | Canonical page URL       |
| `html` | TEXT    | Downloaded HTML document |

The `id` column uniquely identifies each stored page, while the URL and HTML content are used by the Indexer during document processing.

---

# 5.8 Page Retrieval

When the crawler requests a cached page, the PageStore retrieves the corresponding HTML document from SQLite.

Workflow:

```text
URL
 │
 ▼
SQLite Query
 │
 ▼
Matching Row
 │
 ▼
Return HTML
```

This allows previously downloaded pages to be reused without issuing additional HTTP requests.

---

# 5.9 Interaction Between Components

The interaction between the crawler, PageStore, and SQLite is illustrated below.

```text
                WebCrawler
                     │
          save(Page) │ fetch(URL)
                     ▼
               PageStore
          ┌──────────┴──────────┐
          │                     │
          ▼                     ▼
     Memory Buffer        SQLite Database
          │                     ▲
          └────── flush() ──────┘
```

The crawler interacts only with the `PageStore`. All database operations are encapsulated within the storage module, keeping SQL logic separate from the crawling process.

---

Perfect. This is the most important section because this is what you'll likely be asked about in your viva. I've based these design decisions on the implementation you've shown throughout the project.

---

# 6. Design Decisions

The crawler was designed with modularity, performance, and maintainability as the primary goals. Several design decisions were made during development to improve the efficiency and reliability of the crawler.

---

# 6.1 Why Breadth-First Search (BFS)?

The crawler explores the web using **Breadth-First Search (BFS)**.

A queue-based frontier naturally implements BFS, allowing pages closer to the seed URL to be visited before pages at deeper levels.

### Advantages

* Easy depth control.
* Predictable crawling order.
* Good coverage around the seed URL.
* Simple implementation.

---

# 6.2 Why a Frontier Class Instead of a Queue?

Although the crawler internally uses a queue, it is wrapped inside a dedicated **Frontier** class.

Instead of writing

```cpp
std::queue<CrawlTask> frontier;
```

the crawler interacts with

```cpp
Frontier frontier;
```

### Advantages

* Hides queue implementation.
* Keeps crawler independent of the container.
* Allows future replacement of the queue without modifying crawler logic.
* Improves readability.

This follows the **Abstraction Principle**, where implementation details are hidden behind a simple interface.

---

# 6.3 Why a VisitedSet Class?

Duplicate detection is encapsulated inside the **VisitedSet** class rather than exposing the underlying hash table.

Instead of directly manipulating a hash map throughout the crawler, all duplicate checking is performed through the VisitedSet interface.

### Advantages

* Cleaner crawler implementation.
* Better modularity.
* Easier testing.
* Underlying data structure can be replaced without changing crawler code.

Using a hash table provides **O(1)** average lookup and insertion time.

---

# 6.4 Why Normalize URLs Before Duplicate Detection?

The same web page can often be represented by multiple URLs.

Example

```text
https://Example.com

https://example.com/

https://example.com:443

https://example.com#
```

Without normalization, these URLs would be treated as different pages.

Normalization converts them into a single canonical representation before checking the VisitedSet.

### Advantages

* Prevents duplicate crawling.
* Reduces unnecessary downloads.
* Saves storage space.
* Improves crawling efficiency.

---

# 6.5 Why Separate the Fetcher?

Networking is isolated inside the **Fetcher** module.

The crawler does not directly perform HTTP requests.

Instead, it simply requests

```cpp
fetcher.fetch(url);
```

### Advantages

* Follows the Single Responsibility Principle.
* Simplifies testing.
* Makes networking independent of crawling logic.
* Allows future replacement of the HTTP library.

---

# 6.6 Why Use SQLite?

The crawler stores downloaded pages using **SQLite**.

SQLite was selected because it is lightweight, portable, and serverless.

### Advantages

* No database server required.
* Single database file.
* Simple deployment.
* Reliable persistent storage.
* SQL querying support.
* ACID-compliant transactions.

SQLite is well suited for a desktop crawler where moderate amounts of data are stored locally.

---

# 6.7 Why Use a PageStore?

The crawler never communicates with SQLite directly.

Instead, all database operations are performed through the **PageStore**.

### Advantages

* Separation of concerns.
* Database logic isolated.
* Easier maintenance.
* Cleaner crawler implementation.

The crawler only needs to call

```cpp
save(page);

fetch(url);

isCached(url);
```

without knowing how the database works internally.

---

# 6.8 Why Buffer Pages Before Writing?

Instead of writing every downloaded page immediately to SQLite, the crawler stores new pages in an in-memory buffer.

Only when the buffer reaches a predefined size or the crawl completes are the pages written to the database.

### Advantages

* Reduces disk I/O.
* Fewer database transactions.
* Faster crawling.
* Better overall performance.

Batch writes are significantly more efficient than performing a database write after every page.

---

# 6.9 Why Check the Cache Before Downloading?

Before issuing a network request, the crawler checks whether the requested page already exists in the database.

If the page is available locally, it is read directly from SQLite instead of downloading it again.

### Advantages

* Eliminates redundant network requests.
* Reduces crawling time.
* Conserves bandwidth.
* Reuses previously downloaded data.

This cache-first strategy improves efficiency, particularly when crawling overlapping portions of the web.

---

# 6.10 Why Use SQLite Transactions?

Buffered pages are written to SQLite inside a database transaction.

Instead of committing every page individually, multiple pages are inserted within a single transaction.

### Advantages

* Faster batch insertion.
* Reduced transaction overhead.
* Improved database consistency.
* Automatic rollback on failure.

Transactions ensure that either all buffered pages are stored successfully or none are, preventing partial updates.

---

# 6.11 Why Provide Two Constructors?

The crawler provides both a default constructor and a parameterized constructor.

The default constructor initializes sensible crawl settings, allowing the crawler to be used immediately without additional configuration.

The parameterized constructor enables users to specify custom crawl depth and page limits.

### Advantages

* Supports quick testing.
* Simplifies demonstration.
* Provides flexibility for different crawling scenarios.

---

# 6.12 Why Mark the Seed URL as Visited Before Crawling?

The seed URL is marked as visited before entering the main crawling loop.

This ensures that if the seed page is encountered again through hyperlinks, it is not reinserted into the frontier.

### Advantages

* Prevents duplicate processing of the seed page.
* Simplifies duplicate detection.
* Avoids unnecessary queue operations.

---

# 6.13 Why Store Pages Before Extracting Links?

After downloading a page, the crawler stores it before beginning hyperlink extraction.

This ensures that every successfully downloaded page is persisted even if link extraction or later processing encounters an error.

### Advantages

* Improves reliability.
* Prevents loss of downloaded pages.
* Guarantees that fetched pages are available for indexing.

---

# 7. Time Complexity Analysis

| Operation            | Complexity                    |
| -------------------- | ----------------------------- |
| Frontier Push        | **O(1)**                      |
| Frontier Pop         | **O(1)**                      |
| VisitedSet Lookup    | **O(1)** average              |
| VisitedSet Insert    | **O(1)** average              |
| URL Normalization    | **O(L)**                      |
| Link Extraction      | **O(N)**                      |
| Page Buffer Insert   | **O(1)**                      |
| SQLite Cache Lookup  | **O(log M)** (indexed lookup) |
| Page Fetch (Network) | **Network-dependent**         |

Where:

* **N** = Length of the HTML document.
* **L** = Length of the URL.
* **M** = Number of stored pages.

---

# 8. Error Handling

The crawler includes mechanisms to handle common runtime failures gracefully.

### Network Errors

* Failed HTTP requests are detected by the `Fetcher`.
* Invalid responses are ignored, allowing the crawler to continue processing other pages.

### Duplicate URLs

* The `VisitedSet` prevents already visited URLs from being processed again.

### Invalid Links

* Malformed or unsupported URLs are discarded during URL processing.

### Database Errors

* SQLite operations are executed through the `PageStore`.
* Transaction rollback ensures that partial writes do not corrupt the database.

### Crawl Limits

* The crawler enforces both maximum crawl depth and maximum page limits to prevent uncontrolled execution.

---

# 9. Conclusion

The crawler forms the first stage of the search engine pipeline by discovering web pages, downloading their content, extracting hyperlinks, and storing the collected data for indexing.

The implementation adopts a modular architecture in which networking, frontier management, duplicate detection, URL processing, and storage are implemented as independent components. This separation improves maintainability, simplifies testing, and enables future extensions without affecting the overall system design.

Performance is improved through URL normalization, efficient duplicate detection, buffered database writes, cache-first page retrieval, and transaction-based persistence. Together, these design choices reduce unnecessary network requests, minimize database overhead, and ensure reliable storage of downloaded pages.

Overall, the crawler provides a robust and extensible foundation for the search engine. Its modular design, efficient storage strategy, and clear separation of responsibilities make it well suited for integration with the Indexer and for future enhancements such as multithreading, crawl scheduling, and distributed crawling.