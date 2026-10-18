# Search Engine from Scratch

A C++20 search engine built on my own [data structures library](https://github.com/himanshuthakur113/data_structures-library). The crawler downloads pages and stores them in SQLite, the indexer builds an **inverted index** (word -> pages containing it), and the search program answers queries from that index.

This is **Stage 2 and 3** of the project:

```
Data Structures Library  ->  Crawler + Indexer  ->  Search
        (done)                  (done)             (in progress)
```

## How it works

```
seed URL
   |
   v
Frontier (queue) --> Fetcher (libcurl) --> link extractor
   ^                      |                     |
   |                      v                     v
   |                 PageStore            URL resolve + normalize
   |             (buffer -> SQLite)             |
   |                      |               VisitedSet (HashMap)
   +----------------------+---------------------+
                          |
                          v
   HTML parser -> Tokenizer -> Normalizer -> InvertedIndex -> IndexStore (SQLite)
```

### Crawler

| Component | What it does | Built with |
|---|---|---|
| `Fetcher` | Downloads a page over HTTP(S) | libcurl |
| `URL` | Resolves relative links and normalizes URLs (lowercase host, default ports, `.`/`..` segments, fragments) | `DA` |
| `Frontier` | Queue of `CrawlTask` (URL + depth) waiting to be crawled | `queue` |
| `VisitedSet` | Remembers URLs already seen so nothing is crawled twice | `HashMap` |
| `PageStore` | Buffers pages in memory, flushes them to SQLite in batches, skips duplicates | `DA`, `HashMap`, SQLite |
| `WebCrawler` | Runs the loop with a depth limit and a page limit | all of the above |

### Indexer

| Component | What it does |
|---|---|
| `HTMLParser` | Strips tags, ignores `<script>` and `<style>`, decodes entities |
| `Tokenizer` | Splits text into words |
| `Normalizer` | Lowercases, trims punctuation, removes stopwords, short words and pure numbers |
| `PostingList` | The IDs of the pages one word appears in, without duplicates |
| `InvertedIndex` | `HashMap<word, PostingList>` |
| `IndexStore` | Saves the index to SQLite |

### Search

| Component | What it does |
|---|---|
| `IndexStore::load()` | Reads the saved index back from SQLite into an `InvertedIndex` |
| `SearchEngine` | Turns a query into terms with the same tokenizer and normalizer the indexer used, then returns the pages that contain **all** the terms |

Results are currently listed in the order the pages were crawled. Ranking is the next step.

## Build

Requirements: C++20 compiler, CMake 3.15+, libcurl, SQLite3, GoogleTest.

```bash
# Fedora
sudo dnf install gcc-c++ cmake libcurl-devel sqlite-devel gtest-devel
# Ubuntu / Debian
sudo apt install g++ cmake libcurl4-openssl-dev libsqlite3-dev libgtest-dev
```

```bash
cmake -S . -B build
cmake --build build
```

CMake downloads the data structures library automatically (`FetchContent`). To use a local copy instead:

```bash
cmake -S . -B build -DFETCHCONTENT_SOURCE_DIR_DSLIB=/path/to/data_structures-library
```

## Run

Run from the `build/` folder. The databases are written to `../data/` (created automatically).

```bash
cd build
./main
```

By default `main.cpp` crawls `https://quotes.toscrape.com/` (a site made for crawler practice) to depth 2 with at most 50 pages, builds the index, and prints sample keywords. Change the seed, depth and page limit in `main.cpp`.

Then search the index you just built:

```bash
./search            # reads ../data/pages.db and ../data/index.db
```

```
search> albert einstein
20 page(s) found:
  1. https://quotes.toscrape.com/
  2. https://quotes.toscrape.com/author/Albert-Einstein
  ...
```

Type `exit` to quit. A page is a match only if it contains every word of the query. Capitalization and punctuation are ignored, and very common words such as "the" are skipped.

Please be polite when crawling real sites: keep the page limit small and respect each site's `robots.txt`.

## Tests

```bash
cd build
./test_runner
```

Two `FetcherTest` cases download `https://example.com`, so they need an internet connection.

## Project layout

```
include/ , src/        crawler sources; indexer is in Indexer/
test/                  Google Test suites
data/                  generated SQLite databases (git-ignored)
docs/design/           design proposals and memory diagrams
docs/journal/          daily journals
docs/build_logs/       build logs
```

## Roadmap

- [x] Crawler (frontier, visited set, URL normalization, page store)
- [x] Indexer (HTML parser, tokenizer, normalizer, inverted index, SQLite storage)
- [x] Search: find pages that contain all the words of a query
- [ ] Ranking: order results by relevance (word frequency)
