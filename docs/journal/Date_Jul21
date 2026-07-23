# Day 3 Journal

## Section 1 — Specific Bug

### Bug 1: Runtime Error

After integrating the URL normalizer into the crawler, the program terminated with the following runtime error:

```text
basic_string.h: operator[](size_type):
Assertion '_pos <= size()' failed.
```

This indicated that somewhere in the program I was trying to access a character outside the bounds of a `std::string`.

---

## Section 2 — Failed Attempt

Initially, I suspected that the problem was inside the `link_extractor()` function because I had recently modified one of its loop conditions to iterate until `html.size() - 7`. After reviewing the code, I realized that this was not the source of the error.

My next assumption was that the bug was inside the `split_url()` function since it accessed characters using `url[i]`. However, after testing, the program still crashed.

To identify the actual problem, I used **gdb** to debug the program. The debugger showed that the crash occurred when the HTML string was empty. Since `html.size()` returns an unsigned `size_t`, subtracting six from zero resulted in an unsigned underflow, producing a very large positive value. Consequently, the loop condition evaluated to true, and the program attempted to access characters beyond the end of the string.

I fixed the problem by replacing the loop condition

```cpp
i < html.size() - 6
```

with

```cpp
i + 6 < html.size()
```

This guarantees that enough characters remain before accessing them and prevents out-of-bounds access.

---

## Section 3 — Code Reference

**Commit Hash**

`8e578f9 Add URL class for parsing and normalization`
`ae08c51 Implemented url_resovler function`

**Files Modified**

- `include/url_handler.h`
- `src/url_handler.cpp`
- `src/webcrawler.cpp`


---

## Section 4 — Learning Reflection

Today I learned that constructors should focus on object initialization rather than containing large amounts of processing logic. By moving the URL parsing code into a separate `split_url()` function and calling it from the constructor, the class became cleaner and the parsing logic became reusable whenever the URL needed to be parsed again.

I also gained a much better understanding of URL resolution and normalization. Before today, I thought resolving a URL simply meant appending paths together. While implementing `resolve_url()`, I realized that URLs can be absolute, protocol-relative, root-relative, or relative to the current path, and each case must be handled differently. Similarly, I learned that normalization involves much more than converting text to lowercase—it also includes handling default ports, resolving `..` path segments, removing fragments, and ensuring the URL follows a canonical form.

Finally, debugging this runtime error taught me an important lesson about unsigned integer types in C++. I previously overlooked the fact that `std::string::size()` returns a `size_t`. Subtracting a value from zero does not produce a negative number but instead causes an unsigned underflow, resulting in a very large positive value. Using **gdb** to trace the execution helped me understand the root cause instead of relying on guesswork, and it reinforced the importance of checking array bounds correctly.