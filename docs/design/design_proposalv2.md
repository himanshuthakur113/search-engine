# Design Proposal v2 - Web Crawler

---

# Objective

The objective of this project is to build a Web Crawler that starts from a seed URL. It downloads the HTML of each page and extracts the links from that page to discover new pages to visit. It contains a Frontier to manage URLs waiting to be crawled and a Visited Set to avoid revisiting URLs that have already been visited. Every crawled page is stored to disk so it can be used by a later Indexer to build a searchable index. This project demonstrates how early search engines collected web pages before users searched them, like Google's search engine in 1998.

---

# Section 1 — Public API

## 1. Fetcher

```cpp
class Fetcher
{
public:
    Fetcher();
    ~Fetcher();

    std::string fetch(const std::string& url);

private:
    static size_t WriteCallback(char* data, size_t elementSize,
                                 size_t no_of_bytes, void* userdata);
};
```

## 2. CrawlTask

```cpp
struct CrawlTask
{
    std::string url;
    int depth;

    CrawlTask(std::string _url, int _depth);
};
```

## 3. Frontier

```cpp
class Frontier
{
public:
    void push(const CrawlTask& task);
    CrawlTask pop();
    int size() const;
    bool empty() const;

private:
    queue<CrawlTask> frontier;
};
```

## 4. VisitedSet

```cpp
class VisitedSet
{
public:
    void markVisited(const std::string& url);
    bool contains(const std::string& url);
    int size() const;

private:
    HashMap<std::string, int> visited;
};
```

## 5. PageStore

```cpp
class PageStore
{
public:
    PageStore(const std::string& dir = "../pages");

    void save(const std::string& html);
    int pages_saved() const;

private:
    std::string directory;
    int page_cnt;
};
```

## 6. URL

```cpp
class URL
{
public:
    URL(const std::string& baseURL);

    std::string resolve_url(const std::string& url);
    std::string normalize_url(const std::string& url);
    void split_url(const std::string& url);

private:
    std::string scheme;
    std::string host;
    std::string port;
    std::string path;
    std::string query;
    std::string fragment;
};
```

## 7. WebCrawler

```cpp
class WebCrawler
{
public:
    WebCrawler();
    WebCrawler(int _maxDepth, int _maxPages);

    void crawl(const CrawlTask& seedTask);
    DA<std::string> link_extractor(const std::string& html);

private:
    int maxDepth;
    int maxPages;
    Frontier frontier;
    VisitedSet visited;
    PageStore pagestore;
};
```

---

# Section 2 — Complexity Estimates

| Operation | Best | Average | Worst | Why |
|-----------|------|---------|-------|-----|
| `fetch(url)` | O(H) | O(H) | O(H) | Downloads the whole page body, so cost scales with page size H. Also bounded by network connect/transfer time. |
| `Frontier::push()` | O(1) | O(1) | O(1) | Inserts a `CrawlTask` at the rear of the queue. |
| `Frontier::pop()` | O(1) | O(1) | O(1) | Removes the front task from the queue. |
| `Frontier::empty()` | O(1) | O(1) | O(1) | Returns whether the queue is empty. |
| `Frontier::size()` | O(1) | O(1) | O(1) | Returns the maintained size counter. |
| `VisitedSet::contains()` | O(1) | O(1) | O(n) | Average case is a HashMap lookup. Worst case occurs when many URLs collide into the same bucket. |
| `VisitedSet::markVisited()` | O(1) | O(1) | O(n) | Average case inserts into the HashMap in constant time. Worst case depends on bucket collisions. |
| `VisitedSet::size()` | O(1) | O(1) | O(1) | Returns the maintained counter. |
| `URL::split_url()` | O(L) | O(L) | O(L) | Single linear scan over the URL string of length L, using forward-only indices with no backtracking. |
| `URL::resolve_url()` | O(L) | O(L) | O(L) | Constant number of linear scans over strings of length ~L (checking for `://`, locating the base path's last `/`). |
| `URL::normalize_url()` | O(L) | O(L) | O(L) | Constructs a component split (O(L)), applies constant-time-per-character rules to each component, and does a single linear pass for dot-segment collapsing. |
| `link_extractor(html)` | O(H) | O(H) | O(H) | A single forward-only scan over the HTML string of size H; the index never moves backward, so every character is visited at most once regardless of how many `<a href>` tags are found. |
| `PageStore::save()` | O(H) | O(H) | O(H) | Writes the downloaded HTML to disk. |
| `PageStore::pages_saved()` | O(1) | O(1) | O(1) | Returns the maintained page counter. |
| `crawl(seedTask)` | O(P · (H + N · L)) | O(P · (H + N · L)) | O(P · (H + N · L · V)) | For each of the P pages crawled, the page is downloaded and scanned in O(H) time, producing N candidate links. Each link is resolved and normalized in O(L) time and checked against the VisitedSet. HashMap lookup is O(1) on average but degrades toward O(V) per lookup in the worst case. |

### Notation

- **P** = Number of pages crawled.
- **H** = Average size of an HTML page.
- **N** = Average number of links extracted from a page.
- **L** = Average length of a URL.
- **V** = Number of visited URLs.

---

# Section 3 — Internal Representation

### Crawler Flow Diagram

![Crawler Flow Diagram](/Project_2/Crawler_HT/docs/design/memory_diagram/Crawler_flow.jpeg)

### Crawler Memory Diagram

![Crawler Memory Diagram](/Project_2/Crawler_HT/docs/design/memory_diagram/Crawler_memdiagram.jpeg)


---

# Section 4 — Design Decisions

## Frontier Data Structure

The Frontier stores `CrawlTask`s — a URL paired with its crawl depth — waiting to be crawled. It is implemented using the project's own `queue<CrawlTask>`, giving FIFO order. The first task inserted is the first one crawled, which performs a Breadth-First Search (BFS) of the web graph.

A Stack would instead perform a Depth-First Search (DFS). BFS is preferred because it visits pages closer to the seed first, gives better coverage of a site early in the crawl, and avoids sinking the page budget into one deep path before exploring siblings.

## VisitedSet

A `HashMap<std::string, int>` stores every URL that has already been crawled or queued. Before a link is pushed onto the Frontier, `VisitedSet::contains()` is checked; if already present, the link is skipped. Otherwise it is pushed and immediately marked visited.

An array or linked list would need O(n) time per lookup; the HashMap keeps average lookup at O(1), which matters as the visited set grows over a long crawl.

## URL Handling — the `URL` Class

Unlike v1's plan of free-standing `normalize()`/`extractLinks()` functions, URL handling is implemented as a `URL` class. Each `URL` object is constructed from a page's URL string and holds its **scheme, host, port, path, query, and fragment** as private members, split once by `split_url()`.

This has two consequences for how the crawler is structured:

- **One `URL` object per page, not per link.** `crawl()` constructs a single `URL handle_url(task.url)` after fetching each page, then calls `handle_url.resolve_url(rawLink)` once per extracted link. `resolve_url()` reads the base page's own `scheme`/`host`/`path` members directly (via `this`) rather than re-parsing the base URL from scratch on every call.
- **Resolution and normalization are two explicit steps**, not one. `resolve_url()` turns a raw extracted string (which may be relative) into an absolute URL; `normalize_url()` then takes that absolute string, builds its own internal `URL` from it, and applies the canonicalization rules below. `crawl()` calls both in sequence before checking `VisitedSet`.

### Resolution (`resolve_url`)

A raw string from `link_extractor` falls into one of four cases, checked in this order:

1. **Already absolute** — contains `://` anywhere → returned unchanged.
2. **Protocol-relative** (`//host/path`) — the base page's scheme is prepended.
3. **Root-relative** (`/path`) — the base page's scheme and host are prepended; the link replaces the path entirely.
4. **Path-relative** (`contact`, `../contact`) — the base page's path is trimmed back to its last `/` (its "directory"), and the raw link is appended to that. This can leave `..` segments unresolved in the result (e.g. `/products/../contact`) — that is intentional; collapsing is deferred to normalization.

### Normalization (`normalize_url`)

Given an absolute URL, normalization applies, in order:

1. **Default an empty path to `/`** — a bare `https://example.com` is treated as having path `/`, so it normalizes identically to `https://example.com/`.
2. **Lowercase scheme and host** — not the path, since paths can be case-sensitive on real servers.
3. **Strip the port if it is the scheme's default** (`80` for `http`, `443` for `https`) — done after lowercasing, since the comparison is against the lowercased scheme.
4. **Drop the fragment unconditionally.**
5. **Collapse `.` and `..` path segments** — implemented as a single forward scan that keeps a stack of retained segments: a `.` segment is dropped, a `..` segment pops the most recently retained segment (if any), and an empty segment is skipped. This is what resolves the `../contact`-style path left over from `resolve_url`.
6. **Strip a trailing `/` from the path** unless the path is exactly `/`.
7. **Drop the query only if it is empty** (a bare trailing `?`); a non-empty query string is left untouched, including its parameter order.
8. **Reassemble**: `scheme://host[:port]path[?query]`. No fragment is ever included.

Not handled, and deliberately out of scope for this project: percent-encoding normalization (`%2F` vs `%2f`), `www.` vs non-`www` host collapsing, and query-parameter reordering. Each is either low-value for this scope or actively risky to apply blindly (e.g. `www.` and non-`www` can serve genuinely different content on some sites).

## Link Extraction Logic

`link_extractor` is a hand-written character scanner over the HTML string — no regex, no third-party HTML parser. It performs a single forward pass with one index variable that only ever advances, which keeps the whole function O(H) rather than risking re-scanning already-visited text.

For each position, it:

- Checks for the literal sequence `<a ` (angle bracket, `a`, space) to identify a candidate anchor tag, avoiding false positives against tags like `<article>` or `<audio>` that also start with `<a`.
- Requires `href` to appear as the first attribute immediately after `<a ` — attributes appearing before `href` (e.g. `<a class="btn" href="...">`) are not currently matched. This is a known, accepted limitation of the current implementation, not an oversight — see Section 5.
- Scans forward past `href`, `=`, and the opening quote character (`"` or `'`, whichever appears — the delimiter is not hardcoded), then reads until the matching closing quote to capture the raw attribute value.
- Appends the raw string into the returned `DA<std::string>`.

## Fetch Failure Handling

`Fetcher::fetch()` throws `std::runtime_error` on failure — both when `curl_easy_init()` fails to produce a handle, and when `curl_easy_perform()` returns anything other than `CURLE_OK`. `WebCrawler::crawl()` wraps the fetch-and-process block for each task in a `try`/`catch`; on a caught exception, the URL is logged and the loop moves on to the next task in the Frontier via `continue`. A failed fetch does not increment `pagestore.pages_saved()`, since `PageStore::save()` is only reached on a successful fetch.

## Page Storage

Each successfully fetched page's raw HTML is written to its own file (`page_<n>.txt`) inside `PageStore`'s configured directory via `PageStore::save()`. `PageStore` maintains its own `page_cnt`, incremented per saved page and used both as the file-naming counter and as the value `crawl()` checks against `maxPages`.

This differs from v1's plan of a single structured store recording URL, depth, and HTML per record — see Section 5.

## Crawler Termination

The crawler stops when either:

- `pagestore.pages_saved() >= maxPages`, checked at the top of each loop iteration, or
- The Frontier becomes empty.

## Depth Limit

Each `CrawlTask` carries its own `depth`. After a page is successfully fetched and saved, `crawl()` checks `task.depth >= maxDepth`; if so, it `continue`s without extracting or queuing that page's links, so the crawl doesn't go deeper than `maxDepth` from the seed. Links discovered from a page below the limit are queued with `task.depth + 1`.


---

# Section 5 — Divergences from v1

This section exists so the doc stays honest about what was actually built versus what v1 proposed.

| v1 said | v2 (actual) | Why |
|---|---|---|
| `fetch()` returns an empty string on failure | `fetch()` throws `std::runtime_error` | Exceptions surfaced the failure reason (`curl_easy_strerror`) directly, and `crawl()`'s `try`/`catch` handles it without needing a empty-string convention. |
| `Visited` and `Frontier` are free-standing classes matching v1's sketch exactly | Same shape, but `Frontier`/`VisitedSet` wrap the project's own `queue`/`HashMap` rather than `std::queue`/`std::unordered_map` | Consistent with the from-scratch approach used throughout the Collections Library portion of this curriculum. |
| Link extraction uses regex | Hand-written character-by-character scanner | Chosen deliberately over regex  — comparable complexity, avoids `std::regex`'s backtracking overhead.|
