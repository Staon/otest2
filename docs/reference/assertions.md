---
title: Reference of Assertion Functions
permalink: /reference/assertions/
toc: true
---

## Generic Assertions

```c++
bool testAssert(bool condition_);

bool testAssert(const AssertBean& bean_);
```

Assertions for general usage. The assertion passes when the condition is
true. The [`AssertBean`]({{ "api/html/classOTest2_1_1AssertBean.html" | relative_url }})
can be used by helper methods to report some meaningful message.

## Relational Assertions

```c++
template<template<typename, typename> class Comparator_, typename A_, typename B_>
bool testAssertCmp(A_ a_, B_ b_);
```

This assertions checks two values (*a_* and *b_*) by the specified comparator.
The compararator must be a template which can be instantiated with the types
*A_* and *B_*. There are default comparators
[`::OTest2::Equal`]({{ "api/html/structOTest2_1_1Equal.html" | relative_url }}),
[`::OTest2::NotEqual`]({{ "api/html/structOTest2_1_1NotEqual.html" | relative_url }}),
[`::OTest2::Less`]({{ "api/html/structOTest2_1_1Less.html" | relative_url }}),
[`::OTest2::LessOrEqual`]({{ "api/html/structOTest2_1_1LessOrEqual.html" | relative_url }}),
[`::OTest2::Greater`]({{ "api/html/structOTest2_1_1Greater.html" | relative_url }})
and [`::OTest2::GreaterOrEqual`]({{ "api/html/structOTest2_1_1GreaterOrEqual.html" | relative_url }})
implemented in the [otest2/comparison.h]({{ "api/html/comparisons_8h.html" | relative_url }}).

The default implementation compares values by the operators. There is one more
specialization comparing the `const char*` by the `std::strcmp` function.

If the compared type doesn't offer the comparison operator one can implement
very own comparator. Or he can implement a specialization of the existing
comparators.
 
If the assertion fails it prints values of the operands. Hence, the operand
must be printable by the `operator <<` to a C++ stream. If the compared type
is not printable one can implement a specialization of the 
[`::OTest::PrintTrait`]({{ "api/html/structOTest2_1_1PrintTrait.html" | relative_url }}).

Note, that the chosen comparator is printed too. If you implement custom
comparator you have to make its specialization of the `::OTest::PrintTrait`
template too.

```c++
template<typename A_, typename B_>
bool testAssertEqual(A_ a_, B_ b_);

template<typename A_, typename B_>
bool testAssertNotEqual(A_ a_, B_ b_);

template<typename A_, typename B_>
bool testAssertLess(A_ a_, B_ b_);

template<typename A_, typename B_>
bool testAssertLessOrEqual(A_ a_, B_ b_);

template<typename A_, typename B_>
bool testAssertGreater(A_ a_, B_ b_);

template<typename A_, typename B_>
bool testAssertGreaterOrEqual(A_ a_, B_ b_);
```

These assertions are just shortcuts using the default comparators.

### Item-Wise Assertions

```c++
template<template<typename, typename> class Comparator_, typename IterA_, typename IterB_>
bool testAssertItemWise(
    IterA_ begin_a_, IterA_ end_a_, IterB_ begin_b_, IterB_ end_b_);

template<template<typename, typename> class Comparator_, typename ContainerA_, typename IterB_>
bool testAssertItemWise(
    const ContainerA_& a_, IterB_ begin_b_, IterB_ end_b_);

template<template<typename, typename> class Comparator_, typename IterA_, typename ContainerB_>
bool testAssertItemWise(
    IterA_ begin_a_, IterA_ end_a_, const ContainerB_& b_);

template<template<typename, typename> class Comparator_, typename ContainerA_, typename ContainerB_>
bool testAssertItemWise(
    const ContainerA_& a_, const ContainerB_& b_);
```

The item-wise assertions apply the specified relational comparator on each pair
of items with the same index in both lists *a_* a *b_*. The assertion passes if
all pairs pass. If the lists are different size the assertion fails.

The items of the lists must be printable by the `operator <<` or they must
have a specialization of
the [`::OTest::PrintTrait`]({{ "api/html/structOTest2_1_1PrintTrait.html" | relative_url }}).

User can pass just 2 pairs of forward iterators. Or it can pass entire container
instead of the iterator pair. The framework accesses the container's iterators by the 
[`::OTest2::ListContainerTrait`]({{ "api/html/structOTest2_1_1ListContainerTrait.html" | relative_url }}).
The default implementation works with the STL-style methods `begin()` and `end()`.
One can implement custom specialization of the trait. 

### Lexicographical Assertions

```c++
template<template<typename, typename> class Compare_, typename IterA_, typename IterB_>
bool testAssertLexi(
    IterA_ begin_a_, IterA_ end_a_, IterB_ begin_b_, IterB_ end_b_);

template<template<typename, typename> class Compare_, typename ContainerA_, typename IterB_>
bool testAssertLexi(
    const ContainerA_& a_, IterB_ begin_b_, IterB_ end_b_);

template<template<typename, typename> class Compare_, typename IterA_, typename ContainerB_>
bool testAssertLexi(
    IterA_ begin_a_, IterA_ end_a_, const ContainerB_& b_);

template<template<typename, typename> class Compare_, typename ContainerA_, typename ContainerB_>
bool testAssertLexi(
    const ContainerA_& a_, const ContainerB_& b_);
```

The lexicographical assertions check two lists lexicographically. That means
that the algorithm takes one item by one item from the both lists and compares
them. If the items equal the algorithm moves forward to next item. If they
don't the assertion passes or fails according to used comparator. If one list
is the prefix of the other one the comparator must resolve the relation.

There are prepared comparators
[`::OTest2::LexiLess`]({{ "api/html/classOTest2_1_1LexiLess.html" | relative_url }}),
[`::OTest2::LexiLessOrEqual`]({{ "api/html/classOTest2_1_1LexiLessOrEqual.html" | relative_url }}),
[`::OTest2::LexiGreater`]({{ "api/html/classOTest2_1_1LexiGreater.html" | relative_url }})
and [`::OTest2::LexiGreaterOrEqual`]({{ "api/html/classOTest2_1_1LexiGreaterOrEqual.html" | relative_url }})
implemented in the 
[otest2/comparisonslexi.h]({{ "api/html/comparisonslexi_8h.html" | relative_url }}).
The default implementation is a composition of the `::OTest2::Less` and
`::OTest2::Greater` comparators. If you have a specialization of these
comparators you can use the lexicographical comparators even for your
custom types.

A lexicographical comparator must implement two methods:
 * **checkItems()** which takes one item from the left list and one item
   form the right list and returns {-1, 0, 1} in the meaning
   {less, equal, greater}.
 * **checkRests()** taking one boolean for the left list and one for
   the right list. If the boolean is true the appropriate list has ended
   (there is no remaining tail of the list). The function returns true
   if the remaining tails fit the operator.

See the implementation in
the [otest2/comparisonslexi.h]({{ "api/html/comparisonslexi_8h_source.html" | relative_url }})
as an example.

The items of the lists must be printable by the `operator <<` or they must
have a specialization of
the [`::OTest::PrintTrait`]({{ "api/html/structOTest2_1_1PrintTrait.html" | relative_url }}).

User can pass just 2 pairs of forward iterators. Or it can pass entire container
instead of the iterator pair. The framework accesses the container's iterators by the 
[`::OTest2::ListContainerTrait`]({{ "api/html/structOTest2_1_1ListContainerTrait.html" | relative_url }}).
The default implementation works with the STL-style methods `begin()` and `end()`.
One can implement custom specialization of the trait.

### Map Assertions

```c++
template<template<typename, typename> class Compare_, typename ContainerA_, typename ContainerB_>
bool testAssertMap(const ContainerA_& a_, const ContainerB_& b_);
```

The assertion takes two associative containers and compares all items
with the same key by the specified comparator. If a container is a multimap
the items with the same key are compared in the order how they are iterated.

**Note:** the `unordered_multimap` container is not generally supported
as the order of items with the same key is not defined. The container may
be used only if it contains just unique keys. 

The algorithm accesses the containers by the 
[`::OTest2::MapContainerTrait`]({{ "api/html/structOTest2_1_1MapContainerTrait.html" | relative_url }}).
The user may write custom specialization to plug own container in. The default
implementation works with the STL containers `std::map`, `std::multimap`,
`std::unordered_map` and partially with the `std::unordered_multimap`.

The implementation of the trait must offer:
 * a typedef `KeyType`,
 * a typedef `ValueType`,
 * functions `begin` and `end` to access the container's iterators,
 * a function `equalRange` returning range of items for specified key,
 * and a function `keyEqual` comparing keys for equality.

Both keys and value in the containers must be printable by the `operator <<`
or they must have implementation of the
[`::OTest::PrintTrait`]({{ "api/html/structOTest2_1_1PrintTrait.html" | relative_url }}). 

## Regression Assertions

```c++
template<typename Object_>
bool testRegression(const std::string& key_, const Object_& object_);

template<typename Object_>
bool testRegressionP(const std::string& key_, const Object_& object_);

template<typename Object_>
bool testRegressionW(const std::string& key_, const Object_& object_);
```

Regression assertions are used for checking and storing of regression test
marks. The function with the **P** suffix prints current test mark to the
standard output. The function with the **W** suffix stores current test mark
into the regression storage.

The parameter `key_` is an identifier of the stored mark. It must be unique
in one test case. The stored marks are identified by a full key
`'SuiteName'>>'CaseName'>>'key'`. Be careful! If you rename the suite or
the test case the stored marks won't be found and the test will start failing.

The type of the `object_` parameter must implement the method
`test_testMark` or a specialization of the
[`::OTest2::RegressionTrait`]({{ "api/html/structOTest2_1_1RegressionTrait.html" | relative_url}})
must exist for.

Usage of regression test marks is explained in
the [example]({{ "/examples/regressions/" | relative_url }}).
