---
title: Tag Globs and Expressions
permalink: /reference/tagglob/
toc: true
---

# Grammar

A tag glob is a simple sequence of glob parts separated by quad-dot:
```
Glob := <epsilon>
Glob := GlobPart ('::' GlobPart)*
```

The globs are checked for each test case object. The GlobParts must
match tags assigned to appropriate testing objects from the root
to the case. The empty glob matches any object.
```
GlobPart := TagExpression
GlobPart := '[' TagExpression ']'
GlobPart := '[' TagExpression ']' '*'
GlobPart := '*'
GlobPart := '**'
```
The `TagExpression` describes conditions about tags set on the appropriate
testing object. The glob part matches if the object tags match the expression.
If the expression is placed in the brackets it must matches one or more
consequent objects, respectively zero or more times if there is the star after
the brackets.

The simple star means any tag settings. It causes that tags of the appropriate
object always matches even if there is no tag. The double star matches zero
or more consequent objects with any tags.

The `TagExpression` may consist of tag names, the empty class and several
logical operators:
```
TagExpression := <object name>
TagExpression := #tag_name
TagExpression := '<empty>'
TagExpression := '(' TagExpression ')'
TagExpression := '!' TagExpression
TagExpression := TagExpression '&&' TagExpression
TagExpression := TagExpression '||' TagExpression
```
* The object name matches if the name of the object is equal in the
  case sensitive manner. The object name must match regular expression
  `[a-zA-Z_][a-zA-Z0-9_]*`.
* The tag name matches if the tag is set on the object. The tag must match
  regular expression `[a-zA-Z_][a-zA-Z0-9_.\-]*`.
* The class `<empty>` matches if there is no tag set on the object.
* The logical operators have the same meaning like the C/C++ operators.
  Ordered priority of the operators is: `!`, `&&` and `||`.
 
# Examples

 * `MySuite::**` matches all children objects in the root object `MySuite`.
 * `**::#quick-test` matches all test cases (last part of the glob) with
   tag `quick-test`.
 * `**::#extra-suite::**` matches all objects which are tagged or whose
   ancestor is tagged by the `extra-suite`.
 * `[<empty>]` matches all untagged cases in all untagged suites.
 * `[!<empty>]` matches all tagged cases in all tagged suites.
 * `**::#my-tag::*` matches all cases whose first parent has set the tag `my-tag`.
 * `**::#tag1 && #tag2` matches cases with both tags set.
 * `**::#tag1 || #tag2` matches cases with one of the tags.
