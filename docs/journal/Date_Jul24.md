# Design Journal

**Date:** 24/07/2026

---

# Section 1 — Specific Bug

## Bug 1: Fetcher HTML Validation Test Failed

### Test Output

```text
FetcherTest.ReturnedContentContainsHTML
```

The fetcher successfully downloaded the requested page, but one unit test failed because the returned content did not satisfy the expected HTML validation performed by the test.

---

## Bug 2: Duplicate Pages Inside the Page Buffer

### Runtime Output

```text
DB cannot insert page
```

While testing the crawler, I discovered that the `PageStore` buffer could contain multiple copies of the same page. During `flush()`, duplicate pages were inserted into SQLite, causing the database insertion to fail.

---

## Bug 3: High Memory Usage During Crawling

During the design review, I noticed that every fetched page remained in the in-memory buffer until the crawler completed execution. Crawling a large website would continuously increase memory consumption.

---

## Bug 4: HashMap Could Not Be Used with Const Objects

While implementing the `InvertedIndex`, I needed read-only search operations. However, `HashMap::contains()` and `HashMap::find()` were not declared as `const`, preventing them from being called on constant objects.

---

# Section 2 — Failed Attempt

## Bug 1

I initially inspected the Fetcher implementation expecting to find an obvious networking issue.

The fetch operation itself appeared to work correctly, and the HTML was being downloaded successfully. Since I could not determine why the unit test expected different output, I decided to postpone this issue and continue with the remaining test suites so that overall project progress was not blocked.

---

## Bug 2

My first assumption was that duplicate pages were only entering the database.

After tracing the crawler execution, I realized that the duplicate pages already existed inside the `PageStore` buffer before `flush()` was called.

The correct solution was to prevent duplicates from entering the buffer itself by maintaining a `HashMap` of buffered pages.

---

## Bug 3

Initially, I considered increasing the available buffer size.

After reviewing the design, I realized that increasing the buffer would only delay the problem instead of solving it.

A better approach was to periodically flush buffered pages to SQLite after every fixed number of pages, limiting memory usage throughout the crawl.

---

## Bug 4

At first I considered removing `const` qualifiers from the indexing functions so they could call the existing `HashMap` interface.

This would have weakened const-correctness across the project.

Instead, I added proper `const` overloads to both `HashMap` and `LinkedList`, allowing read-only operations without modifying the containers.

---

# Section 3 — Memory Diagram

## Duplicate Buffer Problem

```text
PageStore Buffer

+---------+
| Page A  |
+---------+

+---------+
| Page B  |
+---------+

+---------+
| Page A  |
+---------+

        │
        ▼

Flush()

        │
        ▼

SQLite

INSERT Page A

INSERT Page B

INSERT Page A

↓

Duplicate insertion error
```

---

## Indexer Workflow

```text
Page

   │

   ▼

HTML Parser

   │

   ▼

Plain Text

   │

   ▼

Tokenizer

   │

   ▼

Normalizer

   │

   ▼

Inverted Index

   │

   ▼

Posting List

   │

   ▼

Posting (page_id)
```

---

# Section 4 — Code Reference

## Commit History

```text
a3bd0      feat: refactor crawler processing pipeline
a6a1e55    feat: implemented indexer pipeline
cf6b349    feat: implemented InvertedIndex using HashMap
11e9642    feat: implement PostingList with duplicate page detection
2de94b0    feat: implement basic tokenizer for plain text
ab478db    feat: ignore script and style elements during HTML parsing
6843ca7    feat: implement HTML parser with basic tag removal
aff3b24    fix: PageStore duplicate crash and add SQLite automatic flushing
64f4739    test: completed unit tests for Frontier and VisitedSet
594edbc    test: add ExtractLinks unit tests
675df07    test: add PageStore unit tests
4d817dc    test: add SQLiteDB unit tests and fix fetch query bugs
165b9c3    test: add URL handler unit tests and fix path normalization bug
```

## Files Modified

### Testing

**URL Handler**
- Added comprehensive unit tests for URL parsing, normalization, and URL resolution.
- Fixed the dot-segment path normalization bug discovered during testing.

**SQLiteDB**
- Added unit tests for database operations.
- Fixed issues in page retrieval queries.

**Extract Links**
- Added unit tests covering extraction of multiple HTML links.

**Frontier**
- Added unit tests for queue operations.
- Verified insertion, removal, and traversal.

**VisitedSet**
- Added unit tests for duplicate detection and lookup functionality.

**PageStore**
- Added unit tests for page buffering and database interaction.
- Fixed duplicate page insertion inside the buffer.
- Added duplicate detection using a `HashMap`.
- Implemented automatic flushing after every ten buffered pages.
- Wrapped database writes inside SQLite transactions for improved performance.

---

### Indexer

**HTML Parser**
- Implemented basic HTML tag removal.
- Added support for ignoring `<script>` and `<style>` contents.
- Added HTML comment handling.

**Tokenizer**
- Implemented plain-text tokenization.

**Posting**
- Added the `Posting` structure containing page identifiers.

**PostingList**
- Implemented posting list storage.
- Prevented duplicate page IDs inside posting lists.

**InvertedIndex**
- Implemented an inverted index using the custom `HashMap`.
- Added efficient insertion and search operations.

**Indexer**
- Connected the complete indexing workflow:
  - HTML Parsing
  - Tokenization
  - Normalization
  - Inverted Index construction

---

### Data Structures

**HashMap**
- Added `const` overloads for `contains()` and `find()` to support read-only lookups.

**LinkedList**
- Added a `const` overload for `find()` to support const-correctness.

---

### Crawler

- Refactored the crawler processing pipeline.
- Added `processed_pages` tracking.
- Renamed `pages_saved()` to `buffered_pages()` for clarity.
- Added a function to report the total number of pages stored in the database.