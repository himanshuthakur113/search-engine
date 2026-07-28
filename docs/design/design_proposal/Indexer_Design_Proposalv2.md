# Indexer Design Proposal

**Project:** Project 03 -- Indexer  
**Author:** Himanshu Thakur

---

# Section 1 — Objective

The purpose of the **Indexer** is to read the HTML pages stored in **PageStore**, extract the visible text, preprocess it, and construct an **Inverted Index** that enables efficient keyword-based searching.

The **WebCrawler** downloads HTML pages from the Internet and stores them through `PageStore`. The Indexer reads these stored pages, extracts only the text visible to users, tokenizes the text into words, normalizes each word, and inserts the results into an inverted index. This allows future search queries to locate relevant pages without scanning every document.

---

# Section 2 — Public API

## HTMLParser

```cpp
class HTMLParser
{
public:
    std::string extract_text(const std::string& html);
};
```

## Tokenizer

```cpp
class Tokenizer
{
public:
    DA<std::string> tokenize(const std::string& text);
};
```

## Normalizer

```cpp
class Normalizer
{
public:
    std::string normalize(const std::string& word);
};
```

## Posting

```cpp
struct Posting
{
    size_t page_id;
};
```

## PostingList

```cpp
class PostingList
{
public:
    void add_posting(size_t page_id);
    bool contains(size_t page_id) const;
    void show() const;
};
```

## InvertedIndex

```cpp
class InvertedIndex
{
public:
    void add_word(const std::string& word, size_t page_id);
    bool contains(const std::string& word) const;
    const PostingList* search(const std::string& word) const;
    void show() const;
};
```

## Indexer

```cpp
class Indexer
{
private:
    HTMLParser parser;
    Tokenizer tokenizer;
    Normalizer normalizer;
    InvertedIndex index;

    void index_page(const Page& page);

public:
    void build(PageStore& store);
    const InvertedIndex& get_index() const;
};
```

---

# Section 3 — Internal Representation

## HTMLParser

![HTML Parser Memory Diagram](/Project_2/Crawler_HT/docs/design/memory_diagram/htmlparserv2.jpeg)

---

## PostingList

![Posting List Memory Diagram](docs/design/memory_diagram/postinglist.jpeg)

---


## Indexing Pipeline and Inverted Index

![Indexer Pipeline](/Project_2/Crawler_HT/docs/design/memory_diagram/Indexer.jpeg)

### Indexing Workflow

For every page stored inside **PageStore**, the following pipeline is executed:

```text
Page
  │
  ▼
HTMLParser
  │
  ▼
Plain Text
  │
  ▼
Tokenizer
  │
  ▼
DynamicArray<string>
  │
  ▼
Normalizer
  │
  ▼
InvertedIndex
  │
  ▼
HashMap
  │
  ▼
PostingList
  │
  ▼
Posting(page_id)
```

Each normalized word is inserted into the inverted index.

- If the word already exists, its posting list is updated by appending the page ID 
- Otherwise, a new posting list is created for that word.

---

# Section 4 — Complexity Estimates

| Operation | Best Case | Average Case | Worst Case | Reason |
|-----------|-----------|--------------|------------|--------|
| `build()` | O(T) | O(T) | O(T²) | Reads every page and indexes every token. Worst case occurs if HashMap operations degrade because of excessive collisions. |
| `index_page()` | O(m) | O(m) | O(m²) | Processes every token from one page and inserts it into the inverted index. |
| `extract_text()` | O(n) | O(n) | O(n) | Every HTML character is examined once. |
| `tokenize()` | O(n) | O(n) | O(n) | Scans the extracted text once. |
| `normalize()` | O(k) | O(k) | O(k) | Visits each character of one word. |
| `add_word()` | O(1) | O(1) | O(w) | HashMap insertion. Worst case occurs when all words hash to the same bucket. |
| `search()` | O(1) | O(1) | O(w) | HashMap lookup. Worst case occurs because of collisions. |
| `PostingList::add_posting()` | O(1) | O(1) amortized | O(p) | DynamicArray resize when capacity is exceeded. |
| `PostingList::contains()` | O(1) | O(p) | O(p) | Linear search through the posting list. |
| `InvertedIndex::contains()` | O(1) | O(1) | O(w) | HashMap lookup. |
| `get_index()` | O(1) | O(1) | O(1) | Returns a const reference. |

Where:

- **T** = Total number of tokens across all pages.
- **m** = Number of tokens in one page.
- **n** = Number of characters.
- **k** = Length of one word.
- **w** = Number of distinct indexed words.
- **p** = Number of postings for one word.

### Overall Complexity

| Case | Complexity |
|------|------------|
| Average | **O(T)** |
| Worst | **O(T²)** |

---

# Section 5 — Failure Handling

- Empty HTML → Skip the page.
- Empty extracted text → Skip indexing the page.
- Duplicate words → Prevent duplicate page IDs in the posting list.
- Duplicate pages → Skip pages that have already been indexed.
- Corrupted HTML → Continue processing the remaining pages.
- Missing page → Skip the page.
- Invalid characters → Ignore them during tokenization.

---

# Section 6 — Future Compatibility


The Search Engine will tokenize the user's query, normalize the resulting words, search the inverted index, retrieve the matching page IDs, fetch the corresponding pages from **PageStore**, and display the search results.

---

# Design Decisions

### Why Inverted Index?

An inverted index allows direct lookup of words instead of scanning every stored page for each search query.

### Why HashMap?

The HashMap provides expected **O(1)** insertion and lookup performance, making indexing and searching efficient.

### Why DynamicArray?

Posting lists grow dynamically as more pages contain the same word. A DynamicArray provides efficient amortized append operations while keeping memory contiguous.

### Why PageStore?

Keeping downloaded pages in PageStore allows the index to be rebuilt without crawling the web again.

### Why Separate Components?

Separating the HTML parser, tokenizer, normalizer, and inverted index improves modularity, testing, and maintainability. Future features such as stemming or stop-word removal can be added without modifying the rest of the indexing pipeline.

### Why Store Page IDs?

Posting lists store page IDs instead of full URLs because integer IDs require much less memory and can quickly be mapped back to complete page information.
