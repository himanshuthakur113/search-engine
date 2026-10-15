# Search Engine from Scratch: Crawler and Indexer

A C++20 web crawler and indexer built on my own [data structures library](https://github.com/himanshuthakur113/data_structures-library). The crawler downloads pages, stores them in SQLite, and the indexer builds an **inverted index** (word -> pages containing it) that a search engine can query.

This is **Stage 2** of the project:

```
Data Structures Library  ->  Crawler + Indexer  ->  Search Engine
        (done)                  (this repo)            (next)
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
- [ ] Search engine: query the index and rank results
