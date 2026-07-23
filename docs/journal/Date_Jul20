# Day 2 Journal

## Section 1 — Specific Bug

### Bug 1: Data Structures Library Not Found

While testing the `link_extractor()` function, the project failed to build because the WebCrawler project could not find my Data Structures Library.

### Bug 2: Template Implementation File Not Found

After adding the Data Structures Library as a subdirectory, the compiler produced another error because `linked_list.tpp` could not be found. The template implementation files were located inside the `src` directory, but this directory was not added to the include paths.

### Bug 3: DynamicArray Compilation Error

The `DynamicArray::shrink()` function failed to compile because a pointer was declared inside an `if` block and later accessed outside its scope.

### Bug 4: URL Extraction Returned No Output

While improving the URL extraction logic, the program suddenly stopped printing any URLs. The issue was caused by an incorrect `while` loop condition that used the logical `&&` operator instead of `||` when searching for `"https"`.

Later, while redesigning the extraction logic, I encountered another bug where the extractor produced no output because of a typo. I had accidentally written `!=` instead of `==`.

### Bug 5: Fetch Failure

While testing the complete crawler, `curl_easy_perform()` failed for many extracted URLs because they were not normalized. The fetcher threw an exception, causing the crawler to terminate.

### Bug 6: Pages Saved in Wrong Directory

The downloaded HTML pages were not appearing inside the expected `pages` folder. After investigation, I found that the program had created the directory inside the `build` folder because the executable was running from that location.

---

## Section 2 — Failed Attempt

My first implementation of the URL extractor searched directly for the string `"https"` and copied characters until the closing `>` character. Although this worked for a few links, it completely ignored `http`, relative URLs, and many valid HTML formats.

I then modified the search loop, but the extraction stopped working because I incorrectly used the logical `&&` operator while searching for `"https"`. The loop terminated as soon as one character did not match instead of continuing until the entire sequence was found.

Instead of continuing with this approach, I redesigned the extractor to search for the `href` attribute itself. Once `href` was found, the parser skipped characters until reaching either a single quote or double quote and then copied the URL until the matching closing quote. This approach handled a much wider variety of HTML link formats.

After getting the extractor working, I realized that simply extracting URLs was not enough. The output also contained entries such as `#`, `mailto:`, `tel:`, `javascript:void(0)`, and relative URLs. This led me to decide that the crawler required a dedicated URL normalization component.

---

## Section 3 — Code Reference

**Commit Hash**

`c583f02 Completed link_extractor`

`70052a4 Implement crawl function without max_depth and normalization`

**Files Modified**

- `include/webcrawler.h`
- `src/webcrawler.cpp`
- `include/url_handler.h`
- `src/url_handler.cpp`
- `CMakeLists.txt`
- Data Structures Library `CMakeLists.txt`
- `queue.h`
- `dynamic_array.h`


## Section 4 — Learning Reflection

Today I learned that extracting hyperlinks from HTML is much more complicated than simply searching for `"https"`. Different webpages represent links in different ways, including quoted and unquoted attributes, uppercase HTML tags, relative URLs, and special links such as `mailto:` and `javascript:`. Because of this, I realized that a reliable extractor should focus on parsing the `href` attribute rather than searching for specific URL prefixes.

I also understood the importance of separating responsibilities inside the crawler. Initially, I planned to perform URL parsing and normalization directly inside the `WebCrawler` class, but as the logic became more complex, I realized it would make the crawler difficult to maintain. This led me to design a separate `URL` class responsible for parsing URLs into components, resolving relative URLs, and performing normalization. Separating these responsibilities makes the crawler architecture cleaner and easier to extend.

Another important lesson was related to C++ templates and project organization. I encountered build errors because template implementation files were not included correctly, reinforcing my understanding of how template definitions must be visible during compilation. I also became more comfortable debugging logical errors by tracing loop conditions instead of assuming the overall algorithm was incorrect.