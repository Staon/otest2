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
*A_* and *B_*. There are default comparators `::OTest2::Equal`,
`::OTest2::NotEqual`, `::OTest2::Less`, `::OTest2::LessOrEqual`,
`::OTest2::Greater` and `::OTest2::GreaterOrEqual` implemented
in the [otest2/comparison.h]({{ "api/html/comparisons_8h_source.html" | relative_url }}).

The default implementation compares values by the operators. There is one more
specialization comparing the `const char*` by the `std::strcmp` function.

If the compared type doesn't offer the comparison operator one can implement
very own comparator. Or he can implement a specialization of the existing
comparators.
 
If the assertion fails it prints values of the operands. Hence, the operand
must be printable by the `operator <<` to a C++ stream. If the compared type
is not printable one can implement a specialization of the 
[::OTest::PrintTrait]({{ "api/html/structOTest2_1_1PrintTrait.html" | relative_url }}).

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

The item-wise assertions apply the specified comparator on each pair of items
with the same index in both lists *a_* a *b_*. The assertion passes if
all pairs pass. If the lists are different size the assertion fails.

The items of the lists must be printable by the `operator <<` or they must
have a specialization of
the [::OTest::PrintTrait]({{ "api/html/structOTest2_1_1PrintTrait.html" | relative_url }}).

User can pass just 2 pairs of forward iterators. Or it can pass entire container
instead of the iterator pair. The framework access the container's iterators by the 
[::OTest2::ListContainerTrait]({{ "api/html/structOTest2_1_1ListContainerTrait.html" | relative_url }}).
The default implementation works with the STL-style methods `begin()` and `end()`.
One can implement custom specialization of the trait. 

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
[`RegressionTrait`]({{ "api/html/structOTest2_1_1RegressionTrait.html" | relative_url}})
must exist for.

Usage of regression test marks is explained in
the [example]({{ "/examples/regressions/" | relative_url }}).
