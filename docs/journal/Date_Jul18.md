# Day 1 Journal

## Section 1 — Specific Bug

### Bug 1: Linker Error

While testing the `Fetcher` class, the project compiled successfully but failed during linking because the libcurl library was not linked with the executable.

### Bug 2: CMake Error

After adding

```cmake
target_link_libraries(main PRIVATE CURL::libcurl)
```

CMake produced another error because the `CURL::libcurl` target had not been imported.

---

## Section 2 — Failed Attempt

Initially, I assumed that simply including the header file

```cpp
#include <curl/curl.h>
```

was sufficient to use libcurl. Although the compiler could find the function declarations, the linker failed because the executable was not linked against the libcurl library.

I first added

```cmake
target_link_libraries(main PRIVATE CURL::libcurl)
```

but this still failed because I had not imported the CMake target. After reading the CMake documentation, I realized that `find_package(CURL REQUIRED)` must be called before `target_link_libraries()`. Once I added it, the project built successfully.


---

## Section 3 — Code Reference

**Commit Hash**

`dc433f6 Implement Fetcher using libcurl`

`84287a2 Make change in cmakelist and basic link_extractor idea`

**Files Modified**

- `include/webcrawler.h`
- `include/fetcher.h`
- `src/fetcher.cpp`
- `src/main.cpp`
- `CMakeLists.txt`

---

## Section 4 — Learning Reflection

Today I learned the difference between **compilation** and **linking**. Before today, I thought that including a library header was enough to use an external library. After debugging the linker error, I understood that the compiler only checks whether the function declarations exist, while the linker is responsible for finding the actual implementations inside the library. This helped me better understand how CMake and external libraries work together.

I also learned how the **libcurl Easy Interface** performs network requests. Initially, I expected the downloaded HTML to be returned directly, but I learned that libcurl instead uses callback functions to decide where the received data should be stored. Understanding the purpose of `CURLOPT_WRITEFUNCTION` and `CURLOPT_WRITEDATA` gave me a much better understanding of how network libraries separate data transfer from data storage.

Another important design decision I made today was to reuse my existing `Queue` implementation as the crawler's frontier instead of creating another wrapper around `LinkedList`. This made the design simpler and reinforced my understanding of the **Rule of Zero**, where classes that contain members managing their own resources generally do not require custom destructors, copy constructors, or copy assignment operators.