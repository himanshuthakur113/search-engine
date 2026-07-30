# Indexer Design Proposal

**Project:** Project 03 – Indexer

**Author:** Himanshu Thakur

---
# Section 1 -Objective

The purpose of the Indexer is to use the raw HTML pages collected by the crawler to make a searchable data structure.

The crawler stores pages exactly as they appear on the web. Searching directly through thousands or millions of HTML pages would require scanning every page for every query, making the search engine extremely slow.

The Indexer solves this problem by preprocessing all stored pages once and creating an **Inverted Index**, which maps every word to the pages where it appears.

The generated index will later be consumed by the Retrieval Engine to answer search queries efficiently.

---

# Section 2 — Public API

## HTMLParser

```cpp
class HTMLParser
{
public:

    std::string extractText(const std::string& html);

};
```

---

## Tokenizer

```cpp
class Tokenizer
{
public:

    DynamicArray<std::string>
    tokenize(const std::string& text);

};
```

---

## Normalizer

```cpp
class Normalizer
{
public:

    std::string
    normalize(const std::string& word);

};
```

---

## InvertedIndex

```cpp
struct Posting
{
    int pageID;
};

class InvertedIndex
{
public:

    void addWord(
        const std::string& word,
        int pageID
    );

    bool contains(
        const std::string& word
    );

    DynamicArray<Posting>
    getPostingList(
        const std::string& word
    );

    void save(
        const std::string& filename
    );

    void load(
        const std::string& filename
    );

};
```

---

## Indexer

```cpp
class Indexer
{
public:

    void build();

private:

    HTMLParser parser;

    Tokenizer tokenizer;

    Normalizer normalizer;

    InvertedIndex index;

};
```

---

# Section 3 — Internal Representation

## HTMLParser

Stores no persistent data.

Processes one HTML document at a time.
![html parser](/Project_2/Crawler_HT/docs/design/memory_diagram/htmlparser.jpeg)

## Inverted Index

![inverted index](/Project_2/Crawler_HT/docs/design/memory_diagram/Indexer.jpeg)

---

# Section 4 — Failure Handling

## Empty HTML

Skip page.

---

## Empty Text

Skip page.

---

## Duplicate Words

If the same word appears multiple times on a page, initially allow duplicate insertions.

Future versions will replace duplicates with frequency counts.

---

## Corrupted Page

Log the error and continue indexing remaining pages.

---

## Missing Page

Skip page and continue processing.

---

## Invalid Characters

Ignore non-alphabetic characters while tokenizing.

---

# Section 5 — Complexity Analysis

| Operation | Best | Average | Worst |
|-----------|------|----------|-------|
| Read Page | O(1) | O(1) | O(1) |
| Strip HTML | O(n) | O(n) | O(n) |
| Tokenize | O(n) | O(n) | O(n) |
| Normalize | O(k) | O(k) | O(k) |
| Hash Lookup | O(1) | O(1) | O(n) |
| Posting Append | O(1) | O(1) | O(n) |

Overall Complexity

```
O(total characters processed)
```

---

# Section 6 — Future Compatibility

The Retrieval Engine will consume the generated inverted index instead of scanning HTML pages.

Workflow

```
Crawler

↓

Page Storage

↓

Indexer

↓

Inverted Index

↓

Retrieval Engine

↓

User Search
```

Searching will perform the following steps:

```
Query

↓

Normalize

↓

HashMap Lookup

↓

Posting List

↓

Page IDs

↓

Retrieve URLs

↓

Display Results
```

No HTML parsing will occur during searching because all preprocessing has already been completed during indexing.

---

# Design Decisions

## Why an Inverted Index?

Searching every HTML page for every query is inefficient.

An inverted index transforms

```
Page

↓

Words
```

into

```
Word

↓

Pages
```

allowing direct lookup.

---

## Why HashMap?

HashMap provides average O(1) lookup and insertion, making it suitable for indexing millions of words.

---

## Why DynamicArray?

Posting lists require frequent append operations and sequential traversal.

DynamicArray provides

- O(1) amortized insertion
- Cache-friendly memory layout
- Efficient iteration

---

## Why Separate Components?

Each module has a single responsibility.

- HTMLParser extracts text.
- Tokenizer splits words.
- Normalizer standardizes words.
- InvertedIndex stores mappings.
- Indexer coordinates the workflow.

This modular design improves testing, debugging, and future extensibility.

---

## Why Store Page IDs Instead of URLs?

Page IDs require significantly less memory than storing full URLs in every posting.

URLs can be retrieved later through PageStorage.

---
