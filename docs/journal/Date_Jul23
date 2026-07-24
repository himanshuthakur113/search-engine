# Design Journal

**Date:** 23/07/2026

---

# Section 1 — Specific Bug

## Bug 1: Crawler crashes on second execution

### Runtime Output

```text
SQLite Error: UNIQUE constraint failed: pages.id
```

The crawler worked correctly on the first run but failed during the second execution while flushing the page buffer into the SQLite database. The page IDs started from `0` every time the crawler was executed, causing duplicate primary key values.

---

## Bug 2: Crawler failed after changing the database schema

After modifying the database to use SQLite's auto-increment IDs, the crawler unexpectedly failed on the very first execution.

The issue was traced to the `INSERT` statement where I was still binding the URL and HTML using the old parameter indices after removing the page ID from the SQL query.

---

## Bug 3: URL Normalizer Test Failure

### Test Output

```text
Expected:
https://example.com/c

Actual:
https://example.com/a/..c
```

The normalizer failed to correctly process URLs containing dot-segments (`.` and `..`).

---

# Section 2 — Failed Attempt

## Bug 1

My first attempt was to prevent duplicate pages from being inserted into the page buffer.

I modified `PageStore::save()` so that if a page already existed in the database, it would not be appended to the in-memory buffer.

Although this prevented duplicate storage, it did **not** solve the actual problem. Every new crawler execution still created `Page` objects whose IDs started from `0`. During `flush()`, SQLite still received duplicate primary keys and rejected the insert.

After analyzing the problem, I realized that the crawler should not manage page IDs at all. The database should be responsible for generating unique IDs.

---

## Bug 2

After changing the schema to use SQLite auto-increment IDs, the crawler stopped working completely.

Initially I suspected that the table definition or SQL statement was incorrect.

After debugging the insert function, I discovered that I had removed the page ID from the SQL query but forgot to update the parameter bindings. The URL and HTML were still being bound to the previous indices.

Correcting the parameter indices immediately fixed the problem.

---

## Bug 3

When the URL normalizer test failed, my first assumption was that the URL parser was incorrectly splitting the path.

After printing the normalized output, I noticed that the parser was actually correct and the bug existed inside the dot-segment removal logic.

Updating the normalization algorithm resolved the failing test.

---

# Section 3 — Memory Diagram

## Duplicate Page ID Problem

```text
First Run

Page Objects

+-------+---------+
| id = 0| page A  |
+-------+---------+

+-------+---------+
| id = 1| page B  |
+-------+---------+

            │
            ▼

SQLite Database

+----+----------------+
| 0  | page A         |
| 1  | page B         |
+----+----------------+


Second Run

Page Objects

+-------+---------+
| id = 0| page A  |
+-------+---------+

            │
            ▼

Attempt to INSERT

ID = 0 already exists

↓

UNIQUE constraint failed
```

---

## Updated Design

```text
Crawler

Page Object

+-----------------------+
| URL                   |
| HTML                  |
+-----------------------+

        │
        ▼

SQLite Database

ID generated automatically

+----+----------------+
| 1  | page A         |
| 2  | page B         |
| 3  | page C         |
+----+----------------+
```

---

## Updated Crawler Flow

```text
Frontier

      │

      ▼

Already in Database?

 ┌───────────────┐
 │               │
 │ Yes           │ No
 │               │
 ▼               ▼

Load HTML     Fetch Web Page

 │               │
 │               ▼

 │         Store in Buffer

 │               │

 └──────► Extract Links

              │

              ▼

        Normalize URLs

              │

              ▼

         Push New URLs

              │

              ▼

        Flush Buffer
```

---

# Section 4 — Code Reference

## Commit

```text
a016dcb avoid redundant fetches using SQLite cache
```

## Files Modified

### `src/page_store.cpp`

- Modified `save()` to avoid buffering duplicate pages.
- Added helper functions to check page existence.
- Added helper function to fetch stored pages.

---

### `src/sqlite_db.cpp`

- Removed manual page ID insertion.
- Updated SQL insert query.
- Corrected parameter bindings after removing the ID column.
- Added page retrieval functionality.

---

### `src/crawler.cpp`

- Updated crawler workflow to load existing pages from the database before making network requests.

---

### `src/url_handler.cpp`

- Fixed dot-segment normalization logic.

---

# Section 5 — Learning Reflection

Initially, I viewed the page ID as part of the crawler's responsibility because every `Page` object naturally contained an ID field. While debugging the duplicate insertion problem, I realized that persistence identifiers should be owned by the database rather than by the application. Letting SQLite generate primary keys removed an entire class of synchronization issues between in-memory objects and persistent storage.

Another important realization was that eliminating duplicate database inserts does not necessarily eliminate duplicate computation. The crawler was still downloading pages that already existed locally. This made me rethink the crawler workflow itself. Instead of treating the database only as permanent storage, it became a cache that could be reused to avoid unnecessary network requests and HTML parsing. That change significantly improved the overall design.

Finally, while fixing the URL normalizer, I learned that observing intermediate outputs is often more valuable than immediately changing the implementation. Printing the normalized URL showed that parsing was already correct, allowing me to isolate the bug to the dot-segment removal logic instead of wasting time debugging unrelated components.