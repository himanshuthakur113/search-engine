# Design Journal

---

# Section 1 — Specific Bug

## HTML Parser Failed Unit Tests

### Runtime Output

Three HTML parser tests failed during unit testing:

- `MixedHTML`
- `IgnoreStyle`
- `IgnoreScript`

### Cause

The parser correctly detected the opening `<script>` and `<style>` tags but failed to detect the corresponding closing tags.

For example,

```html
<script>int x = 10;</script><body>Hello</body>
```

Expected output:

```text
Hello
```

Actual output:

```text

```

The issue was inside the `match_tag()` helper function. It was designed for matching opening tags and expected another character after the matched tag.

When matching closing tags such as:

```html
</script>
```

the comparison failed because the helper checked the character after the `>`.

To solve this, I replaced the closing tag detection with `html.compare()` for `</script>` and `</style>`. After making this change, all HTML parser tests passed successfully.

---

# Section 2 — Failed Attempt

Before starting unit testing, I noticed that the generated inverted index still contained poor-quality keywords. Some entries consisted of only a single character, while others contained only numeric values. These words are not useful for searching, so I modified the normalizer to discard tokens whose length is less than two characters and tokens that do not contain at least one alphabetic character.

After making these improvements, I tested the complete pipeline from crawling web pages to storing them in the database and finally building the inverted index. The generated keywords were much cleaner than in previous runs.

I then started testing each module individually using Google Test.

The HTML parser initially failed three test cases because it never exited `SCRIPT` or `STYLE` mode after reading the closing tags. Replacing the closing tag matching with `html.compare()` solved this issue, and all HTML parser tests passed.

Next, the tokenizer failed one test related to alphanumeric tokens. Initially I thought there was a bug in the tokenizer implementation, but after reviewing its design I realized that the tokenizer is intentionally responsible only for extracting words, not symbols or punctuation. Therefore, the implementation was correct and the test expectation was wrong. I updated the test case instead of modifying the tokenizer.

Finally, I tested the remaining modules including the normalizer, posting list, and inverted index. All of them passed their respective unit tests successfully.

---

# Section 3 — Memory Diagram

## Normalizer Memory Diagram

```text
                +-------------------------+
                |      Raw Token          |
                |   "Teacher's!!!"        |
                +-----------+-------------+
                            |
                            v
                Convert to lowercase
                            |
                            v
                +-------------------------+
                |    "teacher's!!!"       |
                +-----------+-------------+
                            |
                            v
                Remove punctuation
                            |
                            v
                +-------------------------+
                |      "teacher's"        |
                +-----------+-------------+
                            |
                            v
                Remove possessive ('s)
                            |
                            v
                +-------------------------+
                |       "teacher"         |
                +-----------+-------------+
                            |
                            v
                Check token length >= 2
                      /             \
                    No               Yes
                    |                 |
                Discard               v
                          Contains alphabet?
                           /          \
                         No            Yes
                         |              |
                     Discard            v
                             Is stop word?
                              /        \
                            Yes        No
                            |           |
                        Discard         v
                           +----------------------+
                           |  Normalized Token    |
                           +----------+-----------+
                                      |
                                      v
                         Insert into Inverted Index
```

### Description

The normalizer receives every token produced by the tokenizer. It converts the token to lowercase, removes punctuation and possessive endings, filters out short tokens and numeric-only tokens, removes stop words, and forwards only valid words to the inverted index.

---

# Section 4 — Code Reference

| Commit Hash | File | Description |
|--------------|------|-------------|
| `8455048` | `src/Indexer/normalizer.cpp` | Added filtering for short tokens and numeric-only tokens. |
| `11db403` | `src/Indexer/html_parser.cpp` | Fixed detection of `</script>` and `</style>` closing tags. |
| `ca5f0f0` | `tests/indexer/` | Added and completed unit tests for the tokenizer and normalizer. |

---

# Section 5 — Learning Reflection

Today was mostly about testing rather than adding new features. I first improved the quality of the generated keywords by filtering out single-character words and tokens that only contain numbers. After rebuilding the entire pipeline, the index looked much cleaner than before.

Running the Google Test suite helped me find problems much faster than manually testing the program. The HTML parser failures immediately showed that the parser never returned to text mode after reading the closing `</script>` and `</style>` tags. Fixing that one issue made all three failed test cases pass.

The tokenizer test taught me something different. At first I assumed there was another implementation bug, but after checking my design I realized the tokenizer was doing exactly what I intended—it should only return words, not punctuation or symbols. Instead of changing the implementation, I corrected the test case to match the intended behavior. That reminded me that sometimes the test is wrong, not the code.

By the end of the day I had successfully tested the HTML parser, tokenizer, normalizer, posting list, and inverted index. Passing all these unit tests gave me much more confidence that each individual component works correctly before moving on to larger integration testing.