# Indexer Design Proposal

**Project:** Web Search Engine – Indexer Module
**Version:** 2.0
**Language:** C++17
**Storage:** SQLite3
**Dependencies:** Data Structures Library, SQLite3

---

# 1. Introduction

The Indexer is the second major component of the search engine. While the crawler is responsible for collecting web pages from the Internet, the indexer transforms those raw HTML pages into a searchable data structure.

Searching every HTML document sequentially for every user query is computationally expensive. Instead, the indexer preprocesses all crawled pages and builds an **inverted index**, allowing future searches to locate relevant documents in nearly constant time.

The module operates as an offline process. It reads pages previously stored by the crawler, extracts meaningful text, normalizes it into a consistent representation, and stores the resulting inverted index inside an SQLite database.

---

# 2. Objectives

The objectives of the Indexer are:

* Read every crawled page from the PageStore.
* Remove HTML markup while preserving visible text.
* Break text into searchable tokens.
* Normalize tokens into a consistent format.
* Build an inverted index mapping words to documents.
* Avoid duplicate page IDs inside posting lists.
* Persist the completed index into SQLite.
* Separate each stage of indexing into independent modules.

---

# 3. Scope

The Indexer performs the following tasks:

* Reading stored pages
* HTML parsing
* Text extraction
* Tokenization
* Token normalization
* Inverted index construction
* SQLite persistence

The following are **outside the scope** of this module:

* Web crawling
* Ranking algorithms
* Query processing
* Search result generation
* Snippet generation

These responsibilities belong to other components of the search engine.

---

# 4. Functional Requirements

The system shall:

### FR1

Read all stored pages from the PageStore.

### FR2

Extract visible text from HTML.

### FR3

Ignore HTML tags.

### FR4

Ignore script, style, and comments.

### FR5

Split extracted text into tokens.

### FR6

Normalize every token before indexing.

### FR7

Insert normalized words into the inverted index.

### FR8

Prevent duplicate page IDs within a posting list.

### FR9

Persist the completed inverted index into SQLite.

---

# 5. Non-Functional Requirements

The Indexer should satisfy the following quality attributes.

## Performance

Each page should be processed only once.

## Modularity

Each stage of indexing should exist as an independent component.

## Maintainability

Normalization rules should be easy to extend.

## Scalability

The design should support indexing thousands of pages without changing the architecture.

## Reliability

Malformed HTML should not terminate the indexing process.

---

# 6. Overall Architecture

```text
                    +--------------------+
                    |     PageStore      |
                    +--------------------+
                               |
                               |
                               v
                    +--------------------+
                    |      Indexer       |
                    +--------------------+
                               |
         -----------------------------------------------
         |             |            |                  |
         v             v            v                  v
   HTMLParser --> Tokenizer --> Normalizer --> InvertedIndex
                                                   |
                                                   |
                                                   v
                                             IndexStore
                                                   |
                                                   |
                                                   v
                                             SQLite index.db
```

The Indexer acts as the coordinator of the complete indexing pipeline.

---

# 7. System Workflow

The indexing process follows these steps:

```text
Load Pages
      |
      v
Extract Visible Text
      |
      v
Tokenize Text
      |
      v
Normalize Tokens
      |
      v
Insert into Inverted Index
      |
      v
Save Index to SQLite
```

Each page passes through every stage exactly once.

---

# 8. Module Design

## 8.1 Indexer

### Responsibility

Coordinates the complete indexing process.

### Inputs

* PageStore

### Outputs

* SQLite index database

### Internal Components

* HTMLParser
* Tokenizer
* Normalizer
* InvertedIndex
* IndexStore

### Processing Steps

1. Load all pages.
2. Parse HTML.
3. Tokenize text.
4. Normalize tokens.
5. Insert tokens into inverted index.
6. Persist index.

The Indexer contains no parsing or normalization logic itself. It delegates each task to a specialized class, keeping the orchestration logic clean and modular.

---

## 8.2 HTMLParser

### Responsibility

Convert HTML documents into plain text.

### Input

HTML document.

### Output

Visible text.

### Processing

The parser scans the HTML document character by character while maintaining a parsing state.

It ignores:

* HTML tags
* JavaScript
* CSS
* HTML comments

Only visible text is forwarded to the tokenizer.

### Design Rationale

Separating HTML parsing from tokenization ensures that later modules operate only on meaningful content and remain independent of HTML syntax.

---

## 8.3 Tokenizer

### Responsibility

Split plain text into individual tokens.

### Input

Plain text.

### Output

List of words.

### Processing

Characters are scanned sequentially.

Alphabetic and numeric characters are grouped into words.

Delimiters such as whitespace and punctuation terminate the current token.

Example:

```text
Input

The quick brown fox.

Output

The
quick
brown
fox
```

---

## 8.4 Normalizer

### Responsibility

Convert equivalent words into a single canonical representation.

### Operations

Depending on the implemented rules, the normalizer may:

* Convert to lowercase
* Remove trailing punctuation
* Remove stop words
*

Example:

```text

NorMalizer
↓

normalizer
```

### Benefits

Normalization reduces index size and improves search quality by ensuring that different forms of the same word map to a common representation.

---

## 8.5 Posting

### Responsibility

Represents one occurrence of a term within a document.

### Stored Information

* Page ID

Using page IDs instead of URLs minimizes storage requirements and simplifies comparisons.

---

## 8.6 PostingList

### Responsibility

Store every page containing a particular word.

Example

```text
computer

↓

12
18
44
78
```

### Features

* Maintains unique page IDs
* Supports insertion of new postings
* Prevents duplicate entries

---

## 8.7 InvertedIndex

### Responsibility

Maintain the mapping:

```text
word
↓

posting list
```

Internally the implementation uses:

```text
HashMap<String, PostingList>
```

Example

```text
apple

↓

1
5
9

computer

↓

3
5
10
```

HashMap lookup provides efficient average-case insertion and retrieval, making it suitable for large vocabularies.

---

## 8.8 IndexStore

### Responsibility

Persist the in-memory inverted index to SQLite.

### Benefits

* Data survives program termination.
* Index can be reused without rebuilding.
* SQLite provides ACID-compliant transactions and efficient disk storage for a single-user application.

---

# 9. Class Relationships

```text
Indexer
 │
 ├── HTMLParser
 ├── Tokenizer
 ├── Normalizer
 ├── InvertedIndex
 │        │
 │        └── PostingList
 │                 │
 │                 └── Posting
 │
 └── IndexStore
```

Each class has a single, well-defined responsibility, adhering to the Single Responsibility Principle (SRP).

---

# 10. Data Structures

| Structure     | Purpose                                      |
| ------------- | -------------------------------------------- |
| HashMap       | Maps words to posting lists                  |
| PostingList   | Stores page IDs for a word                   |
| Dynamic Array | Stores tokens and postings during processing |
| SQLite        | Persistent storage of the index              |

---

# 11. Database Design

The `IndexStore` persists the inverted index into `index.db`.

A suitable schema is:

```sql
CREATE TABLE postings(
    word TEXT NOT NULL,
    page_id INTEGER NOT NULL
);
```

Each row represents a single association between a normalized term and a document.

Indexes on the `word` column can be added later to improve query performance.

---

# 12. Sequence Diagram

```text
Indexer
   |
   |----get_all_pages()----------------------> PageStore
   |
   |<-----------Pages-------------------------
   |
   |----extract_text()-----------------------> HTMLParser
   |
   |<-----------Text--------------------------
   |
   |----tokenize()---------------------------> Tokenizer
   |
   |<-----------Tokens------------------------
   |
   |----normalize()--------------------------> Normalizer
   |
   |<-----------Normalized Tokens------------
   |
   |----insert()-----------------------------> InvertedIndex
   |
   |----save()-------------------------------> IndexStore
   |
   |<-----------SQLite------------------------
```

---

# 13. Algorithm

For each page:

```text
Load page

↓

Extract text

↓

Tokenize

↓

Normalize

↓

Insert into inverted index

↓

Repeat

↓

Persist database
```

This pipeline ensures that each page is processed exactly once.

---

# 14. Complexity Analysis

| Operation           | Complexity                  |
| ------------------- | --------------------------- |
| HTML Parsing        | O(n)                        |
| Tokenization        | O(n)                        |
| Normalization       | O(k) per token              |
| HashMap Insert      | O(1) average                |
| Posting List Insert | O(1) average (append/check) |

Where:

* `n` is the size of the document.
* `k` is the length of a token.

---

# 15. Design Decisions

## Why an Inverted Index?

Scanning every document for each search query is inefficient.

An inverted index enables direct access from a word to the set of documents containing that word, significantly reducing query time.

---

## Why Store Page IDs Instead of URLs?

Page IDs are fixed-size integers, making them more memory-efficient and faster to compare than variable-length URL strings. The mapping from page IDs to URLs is maintained separately by the crawler's storage layer.

---

## Why Separate Parsing, Tokenization, and Normalization?

Each stage performs a distinct transformation:

* **HTMLParser** extracts visible text from HTML.
* **Tokenizer** converts text into lexical units.
* **Normalizer** standardizes those units.

This separation improves modularity, testing, and extensibility. New normalization rules or tokenization strategies can be introduced without affecting other components.

---

## Why SQLite?

SQLite was selected because it:

* Requires no server setup.
* Stores the index in a single portable file.
* Supports transactions for reliable writes.
* Is lightweight and suitable for a single-user search engine.

---

## Why an In-Memory Inverted Index Before Persistence?

Building the index in memory allows rapid insertion using hash-based lookups. Once all pages have been processed, the completed structure is written to SQLite in a batch, reducing disk I/O overhead compared to writing after every token.

---

# 16. Error Handling

The Indexer is designed to continue processing wherever possible:

* Empty pages are skipped.
* Empty tokens are ignored.
* Malformed HTML is handled by the parser without terminating the indexing process.
* Duplicate page IDs are not inserted into posting lists.
* Database write failures are reported by the persistence layer.

---

# 17. Testing Strategy

The module should be validated through unit and integration testing.

### HTMLParser

* Extracts visible text correctly.
* Ignores tags, scripts, styles, and comments.

### Tokenizer

* Correctly splits words using delimiters.
* Handles punctuation and whitespace.

### Normalizer

* Converts words to lowercase.
* Applies implemented normalization rules consistently.

### InvertedIndex

* Inserts new terms correctly.
* Appends page IDs to posting lists.
* Prevents duplicate page IDs.

### IndexStore

* Persists postings to SQLite.
* Reloaded data matches the in-memory index.

### Integration

Verify the complete pipeline:

```
HTML Page
    ↓
HTMLParser
    ↓
Tokenizer
    ↓
Normalizer
    ↓
InvertedIndex
    ↓
SQLite
```

The resulting index should accurately represent the contents of the crawled pages.

---

# 18. Future Improvements

Potential enhancements include:

* Positional indexing for phrase queries.
* Term frequency (TF) and inverse document frequency (IDF) statistics.
* Compression of posting lists.
* Parallel indexing using multiple threads.
* Incremental indexing to process only newly crawled pages.
* Unicode and multilingual text normalization.
* Support for advanced stemming and lemmatization algorithms.

---

This proposal reflects the modular architecture of your current indexer implementation, documenting each component's responsibilities, interactions, and design rationale while remaining aligned with the existing crawler infrastructure.
