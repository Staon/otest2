---
title: Repeaters
lesson: 9
---
Repeated run of suites or test cases

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/repeater)

The OTest2 framework offers a simple way how to run one test case several times
with different data. Let's get a simple example:

```c++
#include <otest2/otest2.h>
#include <otest2/repeatervaluesimpl.h>

TEST_SUITE(RepeaterSuite) {
  TEST_CASE(ValueRepeater) {
    ::OTest2::RepeaterValue<int> repeater{1, 2, 3, 5, 8, 13, 21};

    TEST_SIMPLE() {
      std::cout << "repeated case " << repeater.getIndex() << ": " << repeater.getValue() << std::endl;
    }
  }
}

```

If you run this test you'll get similar output:

{% highlight plaintext linenos %}
 =============================== RepeaterSuite ================================
repeated case 0: 1
  ValueRepeater (run: 1)                                              [Passed]
repeated case 1: 2
  ValueRepeater (run: 2)                                              [Passed]
repeated case 2: 3
  ValueRepeater (run: 3)                                              [Passed]
repeated case 3: 5
  ValueRepeater (run: 4)                                              [Passed]
repeated case 4: 8
  ValueRepeater (run: 5)                                              [Passed]
repeated case 5: 13
  ValueRepeater (run: 6)                                              [Passed]
repeated case 6: 21
  ValueRepeater (run: 7)                                              [Passed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Passed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   1                   0                   1
  Cases                    7                   0                   7
  Checks                   0                   0                   0
  Errors                                                           0
  Test total                                                          [Passed]
 ==============================================================================
{% endhighlight %}

As you can see the test case were repeated for each of the values specified
in the initializer of repeater value.

A list of values is not the only one possibility. One can implement custom
repeater class. E.g. you have a list of communication nodes and you want
to test sending of a message between each of the node pairs:

{% highlight c++ %}
#include <otest2/repeater.h>

class RepeaterNodes : public Repeater {
  private:
    int size;
    int left;
    int right;

  public:
    /* -- just for the sake of the otest2 preprocessor */
    explicit RepeaterNodes(
        Runtime&);

    explicit RepeaterNodes(
        int size_) :
      size(size_),
      left(0),
      right(0) {
      /* -- empty size is not valid - at least one run is needed */
      assert(size > 0);

    }

    std::pair<int, int> getPair() const noexcept {
      return std::pair<int, int>(left, right);
    }

    virtual bool hasNextRun(
        const Context& context_) const noexcept {
      return left < size - 1 || right < size - 1;
    }

    static std::shared_ptr<RepeaterNodes> createNext(
        const Context& context_,
        std::shared_ptr<RepeaterNodes> current_,
        Runtime& runtime_) {
      if(current_ == nullptr) {
        return std::make_shared<RepeaterNodes>(runtime_.nodes.size());
      }
      else {
        if(current_->right < current_->size - 1)
          ++current_->right;
        else {
          current_->right = 0;
          ++current_->left;
        }
        return current_;
      }
    }
};
{% endhighlight %}

A repeater class must derive from the `OTest2::Repeater` interface.

The static function `createNextRun` is the main point of interest. This
factory function is invoked at the beginning of each run of the testing
object. The parameter `current_` contains instance of the repeater object
used in previous run or `nullptr` for the very first run. The function
optionally can accept other parameters which must be filled in the initializer
of the repeater fixture.

The virtual method `hasNextRun()` must return `false` if the last item is
the current one stopping so another run of the testing object.

The first constructor is not implemented and it's present just to keep satisfied
the OTest2 preprocessor. Its parameters must be exactly the same as
the additional parameters of the `createNextRun` static function.

The repeater class is then used quite straightforwardly:

{% highlight c++ %}
TEST_SUITE(RepeaterSuite) {
  TEST_CASE(RepeaterNodesCase) {
    Runtime runtime;
    RepeaterNodes repeater(runtime);

    TEST_SIMPLE() {
      auto node_pair_(repeater.getPair());
      runtime.nodes[node_pair_.first]->sendFrom(
          runtime.nodes[node_pair_.second].get());
    }
  }
}
{% endhighlight %}

with similar output:

{% highlight plaintext linenos %}
 =============================== RepeaterSuite ================================
0: a node 0 contacted me
  RepeaterNodesCase (run: 1)                                          [Passed]
0: a node 1 contacted me
  RepeaterNodesCase (run: 2)                                          [Passed]
0: a node 2 contacted me
  RepeaterNodesCase (run: 3)                                          [Passed]
1: a node 0 contacted me
  RepeaterNodesCase (run: 4)                                          [Passed]
1: a node 1 contacted me
  RepeaterNodesCase (run: 5)                                          [Passed]
1: a node 2 contacted me
  RepeaterNodesCase (run: 6)                                          [Passed]
2: a node 0 contacted me
  RepeaterNodesCase (run: 7)                                          [Passed]
2: a node 1 contacted me
  RepeaterNodesCase (run: 8)                                          [Passed]
2: a node 2 contacted me
  RepeaterNodesCase (run: 9)                                          [Passed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Passed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   1                   0                   1
  Cases                    9                   0                   9
  Checks                   0                   0                   0
  Errors                                                           0
  Test total                                                          [Passed]
 ==============================================================================
{% endhighlight %}
 