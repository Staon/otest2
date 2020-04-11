---
title: CMake Integration
permalink: /reference/cmake/
---

### The Package

```cmake
find_package(OTest2)
```

### Linking of the Library

```cmake
target_link_libraries(target libotest2)
```

### Preprocessing Sources

```cmake
target_otest2_sources(
    target
    [DOMAIN domain]
    sources...
)
```
The function process the specified sources by
the [OTest2 preprocessor]({{ "/reference/preprocessor/" | relative_url }})
and it adds the generated sources into the specified target.

The `DOMAIN` parameter is passed into the `-d` switch of the preprocessor.
However, you will need this option highly unlikely.

### Generating Default Main Function

```cmake
target_otest2_main(target)
```

The function generates the `main.cpp` file containing the default `main`
function and it inserts the file into the specified target.
