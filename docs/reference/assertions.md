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

These assertions compare two values by the appropriate relational operator 
(e.g. `==`, `<`, etc.). The types of the values must implement the operator
and they must be printable to a stream by the `operator <<`.

If the type doesn't implement the operator, one can define a specialization
of one of the traits defined in the header
[comparison.h]({{ "api/html/comparisons_8h_source.html" | relative_url }}).
If the type is not printable, the
[PrintTrait]({{ "api/html/structOTest2_1_1PrintTrait.html" | relative_url }})
can be specialized.

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
