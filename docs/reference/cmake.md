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

The testing objects are registered into a registration "book". The domain
is a "name" of the book. That's it, the domain parameter allows to separate
parsed testing objects into different groups. This feature is used by the
OTest2 selftests. There are test cases which run another test cases testing
the framework features. The result of this "sub-tests" is then checked in
the "main tests". 

### Generating Default Main Function

```cmake
target_otest2_main(target)
```

The function generates the `main.cpp` file containing the default `main`
function and it inserts the file into the specified target.
